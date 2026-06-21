#include "TocLog.h"

std::vector<String> webLogBuffer;
TocLog toclog; // Instantiate the global object instance

size_t TocLog::write(uint8_t c) {
    // FIX: Replicate to the physical hardware terminal using Serial, NOT toclog!
    Serial.write(c);

    // 2. Queue into the running string buffer pipeline
    if (c == '\n') {
        // Strip trailing carriage return if present for clean HTML spacing
        if (currentLine.length() > 0 && currentLine.charAt(currentLine.length() - 1) == '\r') {
            currentLine.remove(currentLine.length() - 1);
        }

        // Move complete string line into the web array vector
        webLogBuffer.push_back(currentLine);
        currentLine = ""; // Flush template for next incoming stream

        // Keep buffer contained within memory threshold constraints
        while (webLogBuffer.size() > MAX_LOG_LINES) {
            webLogBuffer.erase(webLogBuffer.begin());
        }
    } else if (c != '\r') {
        currentLine += (char)c;
    }

    return 1;
}

size_t TocLog::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    for (size_t i = 0; i < size; i++) {
        n += write(buffer[i]);
    }
    return n;
}

size_t TocLog::printf(const char *format, ...) {
    char loc_buf[256]; // Buffer size matching formatting lengths
    va_list arg;
    va_start(arg, format);
    int len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
    va_end(arg);

    if (len > 0) {
        write((const uint8_t*)loc_buf, len);
    }
    return len;
}
