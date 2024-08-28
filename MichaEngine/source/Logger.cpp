#include "Logger.h"

#include <fstream>
#include <iostream>
#include <string>

void Logger::deleteLastLog() {
  file.open("../log.txt");
  file << "\n";
  file.close();
}

void Logger::setLogLevel(LOG_LEVEL level) { ll = level; }

void Logger::showLogApp(bool* open_app) {
  ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
  ImGui::Begin("Log", open_app);
  ImGui::End();
  logApp.draw("Log", open_app);
}

void Logger::writeToFile(const std::string& write) {
  file.open("../log.txt",
            std::fstream::in | std::fstream::out | std::fstream::app);
  file << write + "\n";
  file.close();
}
void Logger::writeToWindow(const std::string& write) {
  Logger::logApp.addLog(write.c_str());
}
void Logger::resetLogger() { deleteLastLog(); }
void Logger::log(const std::string& msg, LOG_VALUES severity, LOG_LEVEL level,
                 const char* filename, int line) {
  if (ll < level) return;
  std::string _msg;
  switch (severity) {
    case LOG_VALUES::INFO:
      _msg = "INFO: ";
      break;
    case LOG_VALUES::WARNING:
      _msg = "WARN: ";
      break;
    case LOG_VALUES::ERROR:
      _msg = "ERR: ";
      break;
  }

  _msg.append(msg);
  _msg.append("               >>| ");
  _msg.append(filename);
  _msg.append(":");
  _msg.append(std::to_string(line));
  std::cout << _msg << std::endl;
  writeToWindow(_msg + "\n");
}

void LogApp::addLog(std::string fmt, ...) {
  int old_size = buf.size();
  va_list args;
  va_start(args, fmt);
  buf.appendfv(fmt.c_str(), args);
  va_end(args);
  for (int new_size = buf.size(); old_size < new_size; old_size++)
    if (buf[old_size] == '\n') lineOffsets.push_back(old_size + 1);
}

LogApp::LogApp() {
  autoScroll = true;
  clear();
}

void LogApp::clear() {
  buf.clear();
  lineOffsets.clear();
  lineOffsets.push_back(0);
}

void LogApp::draw(const char* title, bool* open_app) {
  if (!ImGui::Begin(title, open_app)) {
    ImGui::End();
    return;
  }

  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &autoScroll);
    ImGui::EndPopup();
  }

  // Main window
  if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
  ImGui::SameLine();
  bool clear = ImGui::Button("Clear");
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  filter.Draw("Filter", -100.0f);

  ImGui::Separator();

  if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None,
                        ImGuiWindowFlags_HorizontalScrollbar)) {
    if (clear) this->clear();
    if (copy) ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf_start = buf.begin();
    const char* buf_end = buf.end();
    if (filter.IsActive()) {
      for (int line_no = 0; line_no < lineOffsets.Size; line_no++) {
        const char* line_start = buf_start + lineOffsets[line_no];
        const char* line_end = (line_no + 1 < lineOffsets.Size)
                                   ? (buf_start + lineOffsets[line_no + 1] - 1)
                                   : buf_end;
        if (filter.PassFilter(line_start, line_end))
          ImGui::TextUnformatted(line_start, line_end);
      }
    } else {
      ImGuiListClipper clipper;
      clipper.Begin(lineOffsets.Size);
      while (clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
             line_no++) {
          const char* line_start = buf_start + lineOffsets[line_no];
          const char* line_end =
              (line_no + 1 < lineOffsets.Size)
                  ? (buf_start + lineOffsets[line_no + 1] - 1)
                  : buf_end;
          ImGui::TextUnformatted(line_start, line_end);
        }
      }
      clipper.End();
    }
    ImGui::PopStyleVar();

    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
      ImGui::SetScrollHereY(1.0f);
  }
  ImGui::EndChild();
  ImGui::End();
}
