// Render html with mustache injected
void getView(crow::response &res, const std::string &filename, crow::mustache::context &x) {
  res.set_header("Content-Type", "text/html");

  auto text = crow::mustache::load(filename + ".html").render(x);
  res.write(text);

  res.end();
}

// General loader
void sendFile(crow::response &res, const std::string &filename, std::string contentType){
  std::ifstream in(PUBLIC_PATH + filename, std::ifstream::in);
  if(in){
    std::ostringstream contents;
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

// HTML loader
void sendHtml(crow::response &res, std::string filename){
  sendFile(res, filename + ".html", "text/html");
}

// JPEG loader
void sendImage(crow::response &res, std::string filename){
  sendFile(res, "imgs/" + filename, "image/jpeg");
}

// JS loader
void sendScript(crow::response &res, std::string filename){
  sendFile(res, "scripts/" + filename, "text/javascript");
}

// CSS loader
void sendStyle(crow::response &res, std::string filename){
  sendFile(res, "styles/" + filename, "text/css");
}

// SVG loader
void sendSvg(crow::response &res, std::string filename){
  sendFile(res, "svgs/" + filename, "image/svg+xml");
}

// 404 route
void notFound(crow::response &res, const std::string &message) {
  res.code = 404;
  res.write(message +": File was not found!");
  res.end();
}