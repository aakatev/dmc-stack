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

#include "loaders.cpp"
#include "routes.cpp"

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using mongocxx::cursor;

const std::string MONGO_URI = "mongodb://admin:password1@ds151753.mlab.com:51753/mongo-cpp-server";


int main(int argc, char* argv[]) {
  // Crow config
  crow::SimpleApp app;
  // Mustache config 
  crow::mustache::set_base("/usr/src/cpp-server/server-source/public/");
  // MongoDB config
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{MONGO_URI}};
  auto collection = conn["mongo-cpp-server"]["contacts"];
  auto collection2 = conn["mongo-cpp-server"]["history"];

  // Essential Routes
  // e.g. html, css, js, etc 
  loadEssentialRoutes(app);

  // Other routes
  CROW_ROUTE(app, "/add_json")
  .methods("POST"_method)
  ([&collection](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x)
      return crow::response(400);
    // int sum = x["a"].i()+x["b"].i();
    std::ostringstream os;
    os << x;

    collection.insert_one(bsoncxx::from_json(os.str()));

    return crow::response{os.str()+"\n"};
  });

  CROW_ROUTE(app, "/add_post")
  ([](const crow::request &req, crow::response &res){
    res.set_header("Content-Type", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    sendHtml(res, "add_post");
  });


  CROW_ROUTE(app, "/contact/<string>")
  ([&collection](const crow::request &req, crow::response &res, std::string id){
    auto doc = collection.find_one(make_document(kvp("id", id)));

    if(!doc) {
      return notFound(res, "Contact");
    }

    crow::json::wvalue dto;

    dto["contact"] = crow::json::load(bsoncxx::to_json(doc.value().view()));

    getView(res, "contact", dto);
  });

  CROW_ROUTE(app, "/contacts")
  ([&collection](const crow::request &req, crow::response &res){
    mongocxx::options::find opts;
    opts.limit(10);

    auto docs = collection.find({}, opts);

    crow::json::wvalue dto;
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }
    dto["contacts"] = contacts;

    getView(res, "contacts", dto);
  });

  CROW_ROUTE(app, "/api/contacts")
  ([&collection](const crow::request &req){
    mongocxx::options::find opts;
    opts.limit(10);

    auto docs = collection.find({}, opts);
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }

    crow::json::wvalue dto;
    dto["contacts"] = contacts;

    crow::response res{dto};
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    return res;
  });

  CROW_ROUTE(app, "/api/nba")
  ([&collection2](const crow::request &req){
    mongocxx::options::find opts;
    opts.limit(1);

    auto docs = collection2.find({}, opts);
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(1);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }

    crow::json::wvalue dto;
    dto["contacts"] = contacts;

    crow::response res{dto};
    res.set_header("Content-Type", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    return res;
  });

  CROW_ROUTE(app, "/api").methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get, crow::HTTPMethod::Put)
  ([](const crow::request &req, crow::response &res){
    std::string method = method_name(req.method);
    res.set_header("Content-Type", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    res.write(method + "api");
    res.end();
  });

  CROW_ROUTE(app, "/react")
  ([&collection](const crow::request &req, crow::response &res){
    mongocxx::options::find opts;
    opts.limit(10);

    auto docs = collection.find({}, opts);
    crow::json::wvalue dto;
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }
    dto["contacts"] = contacts;

    getView(res, "react", dto);
  });

  CROW_ROUTE(app, "/contacts/<string>")
  ([&collection](const crow::request &req, crow::response &res, std::string page){
    mongocxx::options::find opts;
    opts.skip(stoi(page)*10);
    opts.limit(10);

    auto docs = collection.find({}, opts);
    crow::json::wvalue dto;
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }
    dto["contacts"] = contacts;

    getView(res, "contacts", dto);
  });

  CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get, crow::HTTPMethod::Put)
  ([](const crow::request &req, crow::response &res){
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    sendHtml(res, "index");
  });

  CROW_ROUTE(app, "/project")
  ([](const crow::request &req, crow::response &res){
    sendHtml(res, "project");
  });

  // Output running port
  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port): 18080);
  std::cout << "Running on PORT " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
