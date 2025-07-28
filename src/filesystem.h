#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <cstdlib>

class FileSystem {
private:
  typedef std::string(*Builder) (const std::string& path);

  static std::string const& getRoot() {
    static char const* envRoot = getenv("LOGL_ROOT_PATH");
    static char const* givenRoot = (envRoot != nullptr ? envRoot : "textures");
    static std::string root = (givenRoot != nullptr ? givenRoot : "");
    return root;
  }

  static Builder getPathBuilder() {
    if (getRoot() != "")
      return &FileSystem::getPathRelativeRoot;
    else
      return &FileSystem::getPathRelativeBinary;
  }

  static std::string getPathRelativeRoot(const std::string& path) {
#ifdef WINDOWS
    return getRoot() + std::string("\\") + path;
#else
    return getRoot() + std::string("/") + path;
#endif
  }

  static std::string getPathRelativeBinary(const std::string& path) {
#ifdef WINDOWS
    return "..\\..\\..\\" + path;
#else
    return "../../../" + path;
#endif
  }

public:
  static std::string getPath(const std::string& path) {
    static std::string(*pathBuilder)(std::string const&) = getPathBuilder();
    return (*pathBuilder)(path);
  }
};

// FILESYSTEM_H
#endif
