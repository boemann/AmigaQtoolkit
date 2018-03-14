#include <proto/dos.h>
#include <dos/dostags.h>

#include "Project.h"


#include <stdio.h>

Project::Project(const AQString &projectPath)
   : m_projectPath(projectPath)
   , m_configuration("release")
{
   loadMakefile();
}

Project::~Project()
{
}

bool advanceToNonWhite(char *&s)
{
   while(*s == ' ' || *s == '\n')
      s++;

   return *s != 0;
} 

void advanceToWhite(char *&s)
{
   while(*s && *s != ' ' && *s != '\n')
      s++;
}

void Project::loadMakefile()
{
   char buf[256];

   AQString makefilePath(m_projectPath);
   makefilePath += "/makefile";
   
   BPTR file =  Open(makefilePath, MODE_OLDFILE);
   if (file == -1)
      return;
   
   char *s;
   bool parsingFiles = false;
   while (FGets(file, buf, 256)) {
      s = buf;
      if (strncmp(buf, "PROG = ", 7) == 0) {
         s = buf + 7;
         advanceToNonWhite(s);
         char *end = s;
         advanceToWhite(end);
         m_projectName = AQString(s, end-s);
      }
      if (strncmp(buf, "SRCS = ", 7) == 0) {
         parsingFiles = true;
         s = buf + 7;
      }
      if (parsingFiles) {
         while (advanceToNonWhite(s)) {
            if (*s != '\\') {
               char *end = s;
               advanceToWhite(end);
               m_files.push_back(AQString(s, end-s));
               s = end;
            }
            if (*s == '\n' || *s == 0)
               break;
            ++s;
         }
         int l = strlen(buf);
         if (l < 2 || buf[strlen(buf) - 2] != '\\')
            break;
      }
   }

   Close(file);
}

AQString Project::projectPath() const
{
   return m_projectPath;
}

int Project::filesCount() const
{
   m_files.size();
}

AQString Project::filename(int index)
{
   if (index < m_files.size())
      return m_files[index];

   return AQString();
}

void Project::build()
{
   AQString cmd("run >nil: runmake ");
   cmd += m_projectPath + " ";
   cmd += m_configuration;
   Execute(cmd, 0, Output());
}

void Project::run()
{
   AQString cmd("run ");
   cmd += m_projectPath;
   cmd += "/";
   cmd += m_configuration;
   cmd += "/";
   cmd += m_projectName;
   SystemTags(cmd, TAG_DONE);
}

