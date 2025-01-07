// @Author Lin Ya
// @Email xxbbb@vip.qq.com
#include "FileUtil.h"   //包含自定义的头文件，可能定义了 AppendFile 类的接口声明
#include <assert.h>     //提供断言功能，用于在运行时检查程序中的逻辑错误
#include <errno.h>      //提供错误代码和错误处理的支持，例如 ferror 会用到
#include <fcntl.h>      //提供与文件控制相关的定义和函数
#include <stdio.h>      //标准输入输出库，包含文件操作的函数，例如 fopen, fclose, fwrite 等
#include <sys/stat.h>   //提供与文件属性相关的函数和定义
#include <unistd.h>     //提供对 POSIX 操作系统 API 的访问，例如文件描述符等操作

using namespace std;

//类的构造函数
AppendFile::AppendFile(string filename) : fp_(fopen(filename.c_str(), "ae")) {
  // 用户提供缓冲区
  setbuffer(fp_, buffer_, sizeof buffer_);
}

AppendFile::~AppendFile() { fclose(fp_); }

void AppendFile::append(const char* logline, const size_t len) {
  size_t n = this->write(logline, len);
  size_t remain = len - n;
  while (remain > 0) {
    size_t x = this->write(logline + n, remain);
    if (x == 0) {
      int err = ferror(fp_);
      if (err) fprintf(stderr, "AppendFile::append() failed !\n");
      break;
    }
    n += x;
    remain = len - n;
  }
}

void AppendFile::flush() { fflush(fp_); }

size_t AppendFile::write(const char* logline, size_t len) {
  return fwrite_unlocked(logline, 1, len, fp_);
}