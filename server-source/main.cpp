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

#include "crow_all.h"
#include "config.h"
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


int main(int argc, char* argv[]) {
  // Crow config
  crow::SimpleApp app;
  // Mustache config 
  crow::mustache::set_base(PUBLIC_PATH);
  // MongoDB config
  mongocxx::instance inst{};
  mongocxx::client conn{mongocxx::uri{MONGO_URI}};
  // MongoDB instance
  auto collection = conn["mongo-cpp-server"]["contacts"];

  // Essential Routes
  // e.g. html, css, js, etc 
  loadEssentialRoutes(app);

  // Other routes
  // Handle json route
  CROW_ROUTE(app, "/add_json")
  .methods("POST"_method)
  ([&collection](const crow::request& req){
    // Get json from req body
    auto x = crow::json::load(req.body);
    if (!x)
      return crow::response(400);
    // Convert json to bson, and insert in db
    std::ostringstream os;
    os << x;
    collection.insert_one(bsoncxx::from_json(os.str()));

    // return json
    return crow::response{os.str()+"\n"};
  });

  // Add contact route
  CROW_ROUTE(app, "/add_contact")
  ([](const crow::request &req, crow::response &res){
    // Set res header
    res.set_header("Content-Type", "text/plain");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    // Reroute to send html route
    sendHtml(res, "add_contact");
  });

  // Contact(phone as query) route
  CROW_ROUTE(app, "/contact/<string>")
  ([&collection](const crow::request &req, crow::response &res, std::string phone){
    // Query contact with same phone
    auto doc = collection.find_one(make_document(kvp("phone", phone)));
    if(!doc) {
      return notFound(res, "Contact");
    }

    // Convert bson to json 
    crow::json::wvalue dto;
    dto["contact"] = crow::json::load(bsoncxx::to_json(doc.value().view()));
    // Render json with mustache
    getView(res, "contact", dto);
  });

  // Contacts route
  CROW_ROUTE(app, "/contacts")
  ([&collection](const crow::request &req, crow::response &res){
    // Specify query options
    mongocxx::options::find opts;
    opts.limit(10);

    // Query data from mongo
    auto docs = collection.find({}, opts);
    crow::json::wvalue dto;
    std::vector<crow::json::rvalue> contacts;
    contacts.reserve(10);

    for(auto doc : docs) {
      contacts.push_back(crow::json::load(bsoncxx::to_json(doc)));
    }
    dto["contacts"] = contacts;
    // Render json with mustache
    getView(res, "contacts", dto);
  });

  // REST API route
  CROW_ROUTE(app, "/api/contacts")
  ([&collection](const crow::request &req){
    // Query data from mongo (similar to Contacts route example)
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

    // Set res header (Essential here, as CORS might be blocked by modern browsers)
    // Attempt to GET data from localhost might also be blocked, so keep it in mind
    crow::response res{dto};
    res.set_header("Content-Type", "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH, DELETE");
    res.set_header("Access-Control-Allow-Headers", "X-Requested-With,content-type");
    res.set_header("Access-Control-Allow-Credentials", "true");
    // Return json
    return res;
  });

  // Index route
  CROW_ROUTE(app, "/").methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get, crow::HTTPMethod::Put)
  ([](const crow::request &req, crow::response &res){
    sendHtml(res, "index");
  });

  // Project route
  CROW_ROUTE(app, "/project")
  ([](const crow::request &req, crow::response &res){
    sendHtml(res, "project");
  });

  // Set app's port (8080 by default)
  // Log port to console
  char* port = getenv("PORT");
  uint16_t iPort = static_cast<uint16_t>(port != NULL? std::stoi(port): 8080);
  std::cout << "DMC Stack app runs on PORT " << iPort << "\n";
  app.port(iPort).multithreaded().run();
}
