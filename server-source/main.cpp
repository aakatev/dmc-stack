#include "crow_all.h"
#include <fstream>
#include <iostream>
#include<vector>
#include<cstdlib>
#include<boost/filesystem.hpp>

#include<bsoncxx/builder/stream/document.hpp>
#include<bsoncxx/json.hpp>
#include<bsoncxx/oid.hpp>

#include<mongocxx/client.hpp>
#include<mongocxx/stdx.hpp>
#include<mongocxx/uri.hpp>
#include<mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using mongocxx::cursor;

using namespace std;
using namespace crow;
using namespace crow::mustache;

const string PUBLIC_PATH = "/usr/src/cpp-server/server-source/public/";
const string MONGO_URI = "mongodb://admin:password1@ds151753.mlab.com:51753/mongo-cpp-server";


void getView(response &res, const string &filename, context &x) {
  res.set_header("Content-Type", "text/html");

  auto text = load(filename + ".html").render(x);
  res.write(text);

  res.end();
}

void sendFile(response &res, const string &filename, string contentType){
  ifstream in(PUBLIC_PATH + filename, ifstream::in);
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
  // Crow config
  crow::SimpleApp app;
  // Mustache config 
  set_base("/usr/src/cpp-server/server-source/public/");
  // MongoDB config
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{MONGO_URI}};
  auto collection = conn["mongo-cpp-server"]["contacts"];

  // Routes
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

  CROW_ROUTE(app, "/contact/<string>")
  ([&collection](const request &req, response &res, string id){
    auto doc = collection.find_one(make_document(kvp("id", id)));
    crow::json::wvalue dto;

    dto["contact"] = json::load(bsoncxx::to_json(doc.value().view()));

    getView(res, "contact", dto);
  });

  CROW_ROUTE(app, "/contacts")
  ([&collection](const request &req, response &res){
    mongocxx::options::find opts;
    opts.limit(10);

    auto docs = collection.find({}, opts);
    crow::json::wvalue dto;
    vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(json::load(bsoncxx::to_json(doc)));
    }
    dto["contacts"] = contacts;

    getView(res, "contacts", dto);
  });


  CROW_ROUTE(app, "/")
  ([](const request &req, response &res){
    sendHtml(res, "index");
  });

  CROW_ROUTE(app, "/about")
  ([](const request &req, response &res){
    sendHtml(res, "about");
  });

  // Output running port
  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? stoi(port): 18080);
  cout << "Running on PORT " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
