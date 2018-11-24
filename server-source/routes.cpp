void loadEssentialRoutes(crow::SimpleApp& app){
  // Source routes
  // CSS route
  CROW_ROUTE(app, "/styles/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendStyle(res, filename);
  });

  // JS route
  CROW_ROUTE(app, "/scripts/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendScript(res, filename);
  });

  // JPEG route
  CROW_ROUTE(app, "/imgs/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendImage(res, filename);
  });

  // SVG route
  CROW_ROUTE(app, "/svgs/<string>")
  ([](const crow::request &req, crow::response &res, std::string filename){
    sendSvg(res, filename);
  });
}