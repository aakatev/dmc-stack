void loadEssentialRoutes(crow::SimpleApp& app){
  // Source routes
  CROW_ROUTE(app, "/styles/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendStyle(res, filename);
  });

  CROW_ROUTE(app, "/scripts/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendScript(res, filename);
  });

  CROW_ROUTE(app, "/imgs/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendImage(res, filename);
  });

  CROW_ROUTE(app, "/svgs/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendSvg(res, filename);
  });
}