
 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;


const int MAX    = 100;      
const int PORT   = 8080;     
const int BUFSZ  = 131072;   
const int ID_START = 1001;  


struct Contact {
    int  id;            
    char name[40];       
    char phone[12];      
    char email[40];      
    char address[60];    
    char city[20];       
};

/* ============================================================
   CLASS : AddressBook
   Core class encapsulating all address book logic.
   Also contains JSON generation methods for web API.
   ============================================================ */
class AddressBook {

private:
    /* -------------------------------------------------------
       PRIVATE DATA MEMBERS  (Encapsulation)
       Cannot be accessed from outside the class.
       ------------------------------------------------------- */
    Contact contacts[MAX];   /* Array of Contact structures     */
    int     count;           /* Current number of contacts      */
    int     nextId;          /* Auto-increment ID counter       */

    int isValidPhone(const char* p) {
        if ((int)strlen(p) != 10) return 0;
        for (int i = 0; i < 10; i++)
            if (!isdigit(p[i])) return 0;
        return 1;
    }

   
    int isDuplicatePhone(const char* p, int skipId = -1) {
        for (int i = 0; i < this->count; i++)
            if (this->contacts[i].id != skipId &&
                strcmp(this->contacts[i].phone, p) == 0)
                return 1;
        return 0;
    }

    
    int findById(int id) {
        for (int i = 0; i < this->count; i++)
            if (this->contacts[i].id == id)
                return i;
        return -1;
    }

    void strToLower(char* dst, const char* src) {
        int i;
        for (i = 0; src[i]; i++)
            dst[i] = (src[i] >= 'A' && src[i] <= 'Z') ? src[i]+32 : src[i];
        dst[i] = '\0';
    }

   
    string jsonEscape(const char* s) {
        string r;
        for (const char* p = s; *p; p++) {
            if      (*p == '"')  r += "\\\"";
            else if (*p == '\\') r += "\\\\";
            else if (*p == '\n') r += "\\n";
            else if (*p == '\r') r += "\\r";
            else if (*p == '\t') r += "\\t";
            else                 r += *p;
        }
        return r;
    }

    
    string contactToJson(const Contact& c) {
        ostringstream oss;
        oss << "{"
            << "\"id\":"       << c.id << ","
            << "\"name\":\""   << jsonEscape(c.name)    << "\","
            << "\"phone\":\""  << jsonEscape(c.phone)   << "\","
            << "\"email\":\""  << jsonEscape(c.email)   << "\","
            << "\"address\":\"" << jsonEscape(c.address) << "\","
            << "\"city\":\""   << jsonEscape(c.city)    << "\""
            << "}";
        return oss.str();
    }

public:

   
    AddressBook() {
        this->count  = 0;
        this->nextId = ID_START;
    }

  
    AddressBook(int startId) {
        this->count  = 0;
        this->nextId = startId;
    }

    
    string getAllJson() {
        ostringstream oss;
        oss << "{\"success\":true,\"count\":" << this->count
            << ",\"max\":" << MAX
            << ",\"contacts\":[";
        for (int i = 0; i < this->count; i++) {
            if (i > 0) oss << ",";
            oss << contactToJson(this->contacts[i]);
        }
        oss << "]}";
        return oss.str();
    }

    
    string addContact(const char* name, const char* phone,
                      const char* email, const char* address, const char* city) {
       
        if (this->count >= MAX)
            return "{\"success\":false,\"message\":\"Address book is full! (100 max)\"}";
        if (strlen(name) == 0)
            return "{\"success\":false,\"message\":\"Name cannot be empty!\"}";
        if (!isValidPhone(phone))
            return "{\"success\":false,\"message\":\"Phone must be exactly 10 numeric digits!\"}";
        if (isDuplicatePhone(phone))
            return "{\"success\":false,\"message\":\"This phone number already exists! Duplicate not allowed.\"}";
	
        Contact c;
        c.id = this->nextId++;
        strncpy(c.name,    name,    39);    c.name[39]    = '\0';
        strncpy(c.phone,   phone,   11);    c.phone[11]   = '\0';
        strncpy(c.email,   email,   39);    c.email[39]   = '\0';
        strncpy(c.address, address, 59);    c.address[59] = '\0';
        strncpy(c.city,    city,    19);    c.city[19]    = '\0';

        this->contacts[this->count] = c;
        this->count++;

        ostringstream oss;
        oss << "{\"success\":true,\"message\":\"Contact added successfully!\""
            << ",\"contact\":" << contactToJson(c) << "}";
        return oss.str();
    }

  
    string updateContact(int id, const char* name, const char* phone,
                         const char* email, const char* address, const char* city) {
        int idx = findById(id);
        if (idx == -1)
            return "{\"success\":false,\"message\":\"Contact not found!\"}";
        if (!isValidPhone(phone))
            return "{\"success\":false,\"message\":\"Phone must be exactly 10 numeric digits!\"}";
        if (isDuplicatePhone(phone, id))
            return "{\"success\":false,\"message\":\"Phone already used by another contact!\"}";

       
        strncpy(this->contacts[idx].name,    name,    39); this->contacts[idx].name[39]    = '\0';
        strncpy(this->contacts[idx].phone,   phone,   11); this->contacts[idx].phone[11]   = '\0';
        strncpy(this->contacts[idx].email,   email,   39); this->contacts[idx].email[39]   = '\0';
        strncpy(this->contacts[idx].address, address, 59); this->contacts[idx].address[59] = '\0';
        strncpy(this->contacts[idx].city,    city,    19); this->contacts[idx].city[19]    = '\0';

        ostringstream oss;
        oss << "{\"success\":true,\"message\":\"Contact updated successfully!\""
            << ",\"contact\":" << contactToJson(this->contacts[idx]) << "}";
        return oss.str();
    }

   
    string deleteContact(int id) {
        int idx = findById(id);
        if (idx == -1)
            return "{\"success\":false,\"message\":\"Contact not found!\"}";

        string deletedName = this->contacts[idx].name;

        for (int i = idx; i < this->count - 1; i++)
            this->contacts[i] = this->contacts[i + 1];
        this->count--;

        ostringstream oss;
        oss << "{\"success\":true,\"message\":\"Contact \\'"
            << jsonEscape(deletedName.c_str()) << "\\' deleted!\","
            << "\"count\":" << this->count << "}";
        return oss.str();
    }

    /* =======================================================
       searchByName() - Public    (Function Overloading Demo)
       Case-insensitive partial match using strstr().
       Returns JSON array of matching contacts.
       Called by: GET /api/search?type=name&q=keyword
       ======================================================= */
    string searchByName(const char* keyword) {
        char lKey[40], lName[40];
        strToLower(lKey, keyword);

        ostringstream oss;
        oss << "{\"success\":true,\"type\":\"name\","
            << "\"keyword\":\"" << jsonEscape(keyword) << "\",\"contacts\":[";

        int found = 0;
        for (int i = 0; i < this->count; i++) {
            strToLower(lName, this->contacts[i].name);
            if (strstr(lName, lKey) != NULL) {
                if (found > 0) oss << ",";
                oss << contactToJson(this->contacts[i]);
                found++;
            }
        }
        oss << "],\"count\":" << found << "}";
        return oss.str();
    }

    
    string searchByPhone(const char* phone) {
        ostringstream oss;
        oss << "{\"success\":true,\"type\":\"phone\","
            << "\"keyword\":\"" << jsonEscape(phone) << "\",\"contacts\":[";

        int found = 0;
        for (int i = 0; i < this->count; i++) {
            if (strcmp(this->contacts[i].phone, phone) == 0) {
                oss << contactToJson(this->contacts[i]);
                found++;
                break;   
            }
        }
        oss << "],\"count\":" << found << "}";
        return oss.str();
    }

    /
    string sortAndGetAll() {
        Contact temp;
        char a[40], b[40];

       
        for (int i = 0; i < this->count - 1; i++) {
            for (int j = 0; j < this->count - i - 1; j++) {
                strToLower(a, this->contacts[j].name);
                strToLower(b, this->contacts[j+1].name);
                if (strcmp(a, b) > 0) {
                    
                    temp                    = this->contacts[j];
                    this->contacts[j]       = this->contacts[j+1];
                    this->contacts[j+1]     = temp;
                }
            }
        }
        return getAllJson();
    }

    /* =======================================================
       getCountJson() - Public
       Returns JSON with total, max, and free slot counts.
       Called by: GET /api/count
       ======================================================= */
    string getCountJson() {
        ostringstream oss;
        oss << "{\"success\":true,"
            << "\"count\":"  << this->count         << ","
            << "\"max\":"    << MAX                  << ","
            << "\"free\":"   << (MAX - this->count)  << "}";
        return oss.str();
    }

}; 
void urlDecode(char* dst, const char* src, int maxLen) {
    int i = 0;
    while (*src && i < maxLen - 1) {
        if (*src == '%' && isxdigit(src[1]) && isxdigit(src[2])) {
            char hex[3] = { src[1], src[2], '\0' };
            dst[i++] = (char)strtol(hex, NULL, 16);
            src += 3;
        } else if (*src == '+') {
            dst[i++] = ' ';
            src++;
        } else {
            dst[i++] = *src++;
        }
    }
    dst[i] = '\0';
}


bool getQueryParam(const char* path, const char* key, char* out, int maxLen) {
    const char* q = strchr(path, '?');
    if (!q) { out[0] = '\0'; return false; }
    q++;

    char search[64];
    snprintf(search, sizeof(search), "%s=", key);
    const char* p = strstr(q, search);
    if (!p) { out[0] = '\0'; return false; }
    p += strlen(search);

    char encoded[512];
    int i = 0;
    while (*p && *p != '&' && i < 511) encoded[i++] = *p++;
    encoded[i] = '\0';

    urlDecode(out, encoded, maxLen);
    return true;
}


bool jsonGetStr(const char* json, const char* key, char* out, int maxLen) {
    char search[64];
    snprintf(search, sizeof(search), "\"%s\":\"", key);
    const char* p = strstr(json, search);
    if (!p) { out[0] = '\0'; return false; }
    p += strlen(search);
    int i = 0;
    while (*p && *p != '"' && i < maxLen - 1) {
        if (*p == '\\') { p++; if (*p) { out[i++] = *p++; } continue; }
        out[i++] = *p++;
    }
    out[i] = '\0';
    return true;
}


bool jsonGetInt(const char* json, const char* key, int* out) {
    char search[64];
    snprintf(search, sizeof(search), "\"%s\":", key);
    const char* p = strstr(json, search);
    if (!p) return false;
    p += strlen(search);
    *out = atoi(p);
    return true;
}


void sendResponse(SOCKET sock, int code, const char* codeText,
                  const char* contentType, const string& body) {
    ostringstream header;
    header << "HTTP/1.1 " << code << " " << codeText << "\r\n"
           << "Content-Type: "   << contentType << "\r\n"
           << "Content-Length: " << body.size() << "\r\n"
           << "Access-Control-Allow-Origin: *\r\n"
           << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
           << "Access-Control-Allow-Headers: Content-Type\r\n"
           << "Connection: close\r\n"
           << "\r\n";

    string h = header.str();
    send(sock, h.c_str(),    (int)h.size(),    0);
    send(sock, body.c_str(), (int)body.size(), 0);
}


void serveFile(SOCKET sock, const char* filePath) {
    ifstream f(filePath, ios::binary);
    if (!f.is_open()) {
        sendResponse(sock, 404, "Not Found", "text/plain",
                     "404 - File not found: " + string(filePath));
        return;
    }
    ostringstream oss;
    oss << f.rdbuf();
    string content = oss.str();

    /* Determine content type from extension */
    const char* ct = "text/html; charset=utf-8";
    if (strstr(filePath, ".css"))  ct = "text/css";
    if (strstr(filePath, ".js"))   ct = "application/javascript";
    if (strstr(filePath, ".ico"))  ct = "image/x-icon";
    if (strstr(filePath, ".png"))  ct = "image/png";

    sendResponse(sock, 200, "OK", ct, content);
}

/* -----------------------------------------------------------
   sendJson()
   Convenience function to send a JSON response.
   ----------------------------------------------------------- */
void sendJson(SOCKET sock, const string& json) {
    sendResponse(sock, 200, "OK", "application/json", json);
}


/* ============================================================
   API HANDLER
   Routes API calls to the correct AddressBook method.
   ============================================================ */
void handleApi(SOCKET sock, const char* method,
               const char* basePath, const char* fullPath,
               const char* body) {

    /* Handle CORS preflight requests */
    if (strcmp(method, "OPTIONS") == 0) {
        sendResponse(sock, 200, "OK", "text/plain", "");
        return;
    }

    /* ------ GET /api/contacts → get all contacts ------ */
    if (strcmp(basePath, "/api/contacts") == 0 &&
        strcmp(method, "GET") == 0) {
        sendJson(sock, g_ab.getAllJson());
        return;
    }

    /* ------ POST /api/contacts → add contact ------ */
    if (strcmp(basePath, "/api/contacts") == 0 &&
        strcmp(method, "POST") == 0) {
        char name[40]="", phone[12]="", email[40]="", address[60]="", city[20]="";
        jsonGetStr(body, "name",    name,    40);
        jsonGetStr(body, "phone",   phone,   12);
        jsonGetStr(body, "email",   email,   40);
        jsonGetStr(body, "address", address, 60);
        jsonGetStr(body, "city",    city,    20);
        sendJson(sock, g_ab.addContact(name, phone, email, address, city));
        return;
    }

    /* ------ PUT /api/contacts → update contact ------ */
    if (strcmp(basePath, "/api/contacts") == 0 &&
        strcmp(method, "PUT") == 0) {
        char idStr[16] = "";
        getQueryParam(fullPath, "id", idStr, 16);
        int id = atoi(idStr);
        if (id == 0) jsonGetInt(body, "id", &id);

        char name[40]="", phone[12]="", email[40]="", address[60]="", city[20]="";
        jsonGetStr(body, "name",    name,    40);
        jsonGetStr(body, "phone",   phone,   12);
        jsonGetStr(body, "email",   email,   40);
        jsonGetStr(body, "address", address, 60);
        jsonGetStr(body, "city",    city,    20);
        sendJson(sock, g_ab.updateContact(id, name, phone, email, address, city));
        return;
    }

    /* ------ DELETE /api/contacts?id=XXXX → delete ------ */
    if (strcmp(basePath, "/api/contacts") == 0 &&
        strcmp(method, "DELETE") == 0) {
        char idStr[16] = "";
        getQueryParam(fullPath, "id", idStr, 16);
        sendJson(sock, g_ab.deleteContact(atoi(idStr)));
        return;
    }

    /* ------ GET /api/search?type=name&q=XXX → search ------ */
    if (strcmp(basePath, "/api/search") == 0 &&
        strcmp(method, "GET") == 0) {
        char type[16]="", q[64]="";
        getQueryParam(fullPath, "type", type, 16);
        getQueryParam(fullPath, "q",    q,    64);

        if (strcmp(type, "name") == 0)
            sendJson(sock, g_ab.searchByName(q));
        else if (strcmp(type, "phone") == 0)
            sendJson(sock, g_ab.searchByPhone(q));
        else
            sendJson(sock, "{\"success\":false,\"message\":\"Invalid search type\"}");
        return;
    }

    /* ------ GET /api/sort → sort contacts ------ */
    if (strcmp(basePath, "/api/sort") == 0 &&
        strcmp(method, "GET") == 0) {
        sendJson(sock, g_ab.sortAndGetAll());
        return;
    }

    /* ------ GET /api/count → contact count stats ------ */
    if (strcmp(basePath, "/api/count") == 0 &&
        strcmp(method, "GET") == 0) {
        sendJson(sock, g_ab.getCountJson());
        return;
    }

    /* ------ 404 for unknown API routes ------ */
    sendResponse(sock, 404, "Not Found", "application/json",
                 "{\"success\":false,\"message\":\"API endpoint not found\"}");
}


/* ============================================================
   HTTP REQUEST HANDLER
   Receives one HTTP request, parses it, and routes it.
   ============================================================ */
void handleClient(SOCKET sock) {
    /* Allocate receive buffer on heap */
    char* buf = new char[BUFSZ];
    memset(buf, 0, BUFSZ);

    /* Receive HTTP request */
    int bytes = recv(sock, buf, BUFSZ - 1, 0);
    if (bytes <= 0) { delete[] buf; return; }
    buf[bytes] = '\0';

    /* Parse HTTP method and full path (with query string) */
    char method[16]   = "";
    char fullPath[512] = "";
    sscanf(buf, "%15s %511s", method, fullPath);

    /* Extract base path (strip query string for routing) */
    char basePath[512] = "";
    strncpy(basePath, fullPath, 511);
    basePath[511] = '\0';
    char* qmark = strchr(basePath, '?');
    if (qmark) *qmark = '\0';

    /* Find HTTP request body (content after \r\n\r\n) */
    const char* bodyStart = strstr(buf, "\r\n\r\n");
    const char* body = bodyStart ? bodyStart + 4 : "";

    /* Log request to console */
    cout << "[" << method << "] " << fullPath << "\n";

    /* ---- ROUTE THE REQUEST ---- */
    if (strcmp(basePath, "/") == 0 ||
        strcmp(basePath, "/index.html") == 0) {
        /* Serve the frontend HTML page */
        serveFile(sock, "public/index.html");

    } else if (strcmp(basePath, "/favicon.ico") == 0) {
        /* Ignore favicon requests */
        sendResponse(sock, 204, "No Content", "text/plain", "");

    } else if (strncmp(basePath, "/api/", 5) == 0) {
        /* Route to REST API handler */
        handleApi(sock, method, basePath, fullPath, body);

    } else {
        /* Try to serve other static files from public/ */
        string filePath = string("public") + basePath;
        serveFile(sock, filePath.c_str());
    }

    delete[] buf;
}


/* ============================================================
   MAIN FUNCTION
   Sets up Winsock, creates the HTTP server socket,
   and runs the accept loop.
   ============================================================ */
int main() {
    cout << "\n";
    cout << "==================================================\n";
    cout << "    ADDRESS BOOK SYSTEM - C++ WEB SERVER\n";
    cout << "==================================================\n";
    cout << "  Backend  : C++ (Winsock2 HTTP Server)\n";
    cout << "  Frontend : HTML5 + CSS3 + JavaScript\n";
    cout << "  API      : REST JSON over HTTP\n";
    cout << "==================================================\n\n";

    /* ---- Initialize Winsock ---- */
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "[ERROR] WSAStartup failed! Code: " << WSAGetLastError() << "\n";
        return 1;
    }

    /* ---- Create server socket ---- */
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSock == INVALID_SOCKET) {
        cout << "[ERROR] Socket creation failed!\n";
        WSACleanup();
        return 1;
    }

    /* ---- Allow port reuse (prevents "Address already in use") ---- */
    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    /* ---- Bind to port ---- */
    sockaddr_in serverAddr;
    serverAddr.sin_family      = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port        = htons(PORT);

    if (bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "[ERROR] Bind failed! Port " << PORT << " may already be in use.\n";
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    /* ---- Start listening ---- */
    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        cout << "[ERROR] Listen failed!\n";
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    cout << "[OK] Server started successfully!\n";
    cout << "[OK] Open browser: http://localhost:" << PORT << "\n\n";
    cout << "[INFO] Waiting for connections... (Press Ctrl+C to stop)\n";
    cout << "--------------------------------------------------\n\n";

    /* Auto-open browser */
    system("start http://localhost:8080");

    /* ---- Main accept loop ---- */
    while (true) {
        SOCKET clientSock = accept(serverSock, NULL, NULL);
        if (clientSock == INVALID_SOCKET) continue;

        handleClient(clientSock);     /* Handle request       */
        closesocket(clientSock);      /* Close client socket  */
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}

/* ============================================================
   END OF SERVER.CPP - ADDRESS BOOK SYSTEM
   ============================================================ */
