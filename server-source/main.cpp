// #include "crow_all.h"
// using namespace std;
// using namespace crow;

// int main(int argc, char* argv[]) {
//   SimpleApp app;

//   CROW_ROUTE(app, "/")
//     ([](const request& req, response& res){
//       ifstream in("/usr/src/cppweb/server-source/public/index.html", ifstream::in);
//       if(in) {
//         stringstream html_content;
//         html_content << in.rdbuf();
//         in.close();
//         res.write(html_content.str());
//       } else {
//         res.write("No .html file found for this route!");
//       }
//       res.end();
//     });

//   char* port = getenv("PORT");
//   uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
//   cout << "PORT = " << iPort << "\n";
//   app.port(iPort).multithreaded().run();
// }

#include "crow_all.h"
using namespace std;
using namespace crow;

const string STATIC = "/usr/src/cppweb/server-source/public/";

void sendFile(response &res, string filename, string contentType){
  cout << filesystem::current_path;
  ifstream in(STATIC + filename, ifstream::in);
  if(in){
    ostringstream contents;
    contents << in.rdbuf();
    in.close();
    res.set_header("Content-Type", contentType);
    res.write(contents.str());
  } else {
    res.code = 404;
    res.write("Not found");
  }
  res.end();
}

void sendHtml(response &res, string filename){
  sendFile(res, filename + ".html", "text/html");
}

void sendImage(response &res, string filename){
  sendFile(res, "imgs/" + filename, "image/jpeg");
}

void sendScript(response &res, string filename){
  sendFile(res, "scripts/" + filename, "text/javascript");
}

void sendStyle(response &res, string filename){
  sendFile(res, "styles/" + filename, "text/css");
}

int main(int argc, char* argv[]) {
  crow::SimpleApp app;

  CROW_ROUTE(app, "/styles/<string>")
    ([](const request &req, response &res, string filename){
      sendStyle(res, filename);
    });

  CROW_ROUTE(app, "/scripts/<string>")
    ([](const request &req, response &res, string filename){
      sendScript(res, filename);
    });

  CROW_ROUTE(app, "/imgs/<string>")
    ([](const request &req, response &res, string filename){
      sendImage(res, filename);
    });

  CROW_ROUTE(app, "/")
    ([](const request &req, response &res){
      sendHtml(res, "index");
    });

  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "PORT = " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
