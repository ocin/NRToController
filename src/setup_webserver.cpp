#include "setup_webserver.h"

WebServer webserver(80); // Start a standard web server on port 80

// CSS Stylesheet based on the dark console theme
const String HTML_STYLE = 
    "<style>"
    "body{font-family:monospace;background:#1e1e1e;color:#d4d4d4;margin:20px;padding:20px;line-height:1.5;}"
    ".card, .console{background:#000;border:1px solid #333;padding:20px;border-radius:5px;max-width:500px;margin:20px auto;box-shadow:0 4px 6px rgba(0,0,0,0.3);}"
    ".console{max-width:800px;max-height:70vh;overflow-y:auto;}"
    "nav{max-width:500px;margin:0 auto 20px auto;text-align:center;display:flex;gap:10px;justify-content:center;}"
    ".nav-btn{background:#2d2d2d;color:#4fc1ff;padding:10px 20px;text-decoration:none;border:1px solid #333;border-radius:4px;font-weight:bold;flex:1;text-align:center;}"
    ".nav-btn:hover{background:#3e3e3e;color:#569cd6;}"
    ".nav-btn.active{background:#007bff;color:white;border-color:#007bff;}"
    "label{display:block;margin-top:15px;color:#9cdcfe;font-weight:bold;}"
    "input[type=text],input[type=number]{width:100%;padding:10px;margin:8px 0;box-sizing:border-box;background:#252526;border:1px solid #3c3c3c;color:#fff;font-family:monospace;border-radius:4px;}"
    "input[type=text]:focus,input[type=number]:focus{border-color:#007bff;outline:none;}"
    "input[type=submit]{width:100%;background:#0e639c;color:white;padding:12px;border:none;border-radius:4px;cursor:pointer;font-family:monospace;font-weight:bold;font-size:14px;margin-top:20px;}"
    "input[type=submit]:hover{background:#1177bb;}"
    "h2{color:#4fc1ff;margin-top:0;text-align:center;border-bottom:1px solid #333;padding-bottom:10px;}"
    "</style>";

// Shared UI Header & Navigation Bar
String getNavigationMenu(String activePage) {
    String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    
    html += HTML_STYLE;
    html += "<title>DCC-EX Controller</title></head><body>";
    
    // Render the layout menu navigation structure
    html += "<nav>";
    html += "<a href='/' class='nav-btn " + String(activePage == "config" ? "active" : "") + "'>Configuration</a>";
    html += "<a href='/logs' class='nav-btn " + String(activePage == "logs" ? "active" : "") + "'>Live Logs</a>";
    html += "</nav>";
    
    return html;
}

// Function to generate the HTML web interface page
void handleRoot()
{
    String html = getNavigationMenu("config");

    html += "<div class='card'><h2>Turnout Controller Config</h2>";
    html += "<form action='/save' method='POST'>";

    // Host and Port Fields
    html += "<label>DCC-EX Host/IP:</label>";
    html += "<input type='text' name='host' value='" + String(tocConfig.dcc_host) + "'>";
    html += "<label>DCC-EX Port:</label>";
    html += "<input type='text' name='port' value='" + String(tocConfig.dcc_port) + "'>";

    // Turnout controller to turnout ID mappings
    for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++)
    {
        html += "<label>Turnout Controller " + String(i + 1) + " Turnout Id:</label>";
        html += "<input type='text' name='turnoutid_" + String(i) + "' value='" + String(tocConfig.turnoutid_mappings[i]) + "'>";
    }

    html += "<input type='submit' value='Save Settings'>";
    html += "</form></div></body></html>";

    webserver.send(200, "text/html", html);
}

// Function to handle incoming form submissions
void handleSave()
{
    if (webserver.method() != HTTP_POST)
    {
        webserver.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    String new_host = webserver.arg("host");
    int new_port = webserver.arg("port").toInt();

    int temp_turnoutid[TocConfig::NUM_TURNOUTS];
    for (int i = 0; i < TocConfig::NUM_TURNOUTS; i++)
    {
        String param_name = "turnoutid_" + String(i);
        temp_turnoutid[i] = webserver.arg(param_name).toInt();
    }

    tocConfig.save(new_host.c_str(), new_port, temp_turnoutid);

    // Dark-themed confirmation response page
    String resp = getNavigationMenu("config");
    resp += "<div class='card'><h2>Settings Saved!</h2><p style='text-align:center;'>Rebooting device to apply changes...</p></div>";
    resp += "<script>setTimeout(function(){window.location.href='/';}, 3000);</script></body></html>";
    webserver.send(200, "text/html", resp);

    delay(1000);
    ESP.restart(); 
}

void handleLogs()
{
    String html = getNavigationMenu("logs");

    html += "<div class='console' id='logConsole'><h2>System Live Logs</h2>";
    // We start with a loading placeholder; JavaScript will fill this immediately
    html += "<div id='logContent'>Loading system diagnostics...</div>";
    html += "</div>";

    // Smooth JavaScript live injector
    html += "<script>";
    html += "function updateLogs() {";
    html += "  fetch('/raw-logs')"; // Request just the raw text string
    html += "    .then(response => response.text())";
    html += "    .then(text => {";
    html += "      const contentDiv = document.getElementById('logContent');";
    html += "      const consoleDiv = document.getElementById('logConsole');";
    html += "      ";
    // Check if the text actually changed to save browser processing power
    html += "      const formattedText = text.replace(/\\n/g, '<br>');";
    html += "      if (contentDiv.innerHTML !== formattedText) {";
    html += "        contentDiv.innerHTML = formattedText;";
    // Smoothly scroll down after updating content
    html += "        consoleDiv.scrollTop = consoleDiv.scrollHeight;";
    html += "      }";
    html += "    })";
    html += "    .catch(err => console.log('Log fetch error:', err));";
    html += "}";
    html += "";
    html += "window.onload = function() {";
    html += "  updateLogs();"; // Run instantly on load
    html += "  setInterval(updateLogs, 1000);"; // Quietly fetch updates every 1 second (1000ms)
    html += "};";
    html += "</script>";
    html += "</body></html>";

    webserver.send(200, "text/html", html);
}

void handleRawLogs() {
    String payload = "";
    for (const String &line : webLogBuffer) {
        payload += line + "\n";
    }
    webserver.send(200, "text/plain", payload);
}

void setup_webserver()
{
    // 3. Display the Local IP Address assigned by your home network router
    toclog.println("\n-------------------------------------");
    toclog.print("Connected! Open this address in your browser: http://");
    toclog.println(WiFi.localIP());
    toclog.println("-------------------------------------\n");

    // 4. Bind the web webserver pathways
    webserver.on("/", handleRoot);
    webserver.on("/save", handleSave);
    webserver.on("/logs", handleLogs);
    webserver.on("/raw-logs", handleRawLogs);
    webserver.on("/favicon.ico", []() { webserver.send(204); });
    webserver.begin(); // Start the background local web service
}
