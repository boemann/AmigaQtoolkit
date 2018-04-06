#include <AQString.h>
#include <vector.h>

class Project
{
public:
   Project(const AQString &projectPath);
   ~Project();

   int filesCount() const;
   AQString filename(int index);

   AQString projectPath() const;

   void setConfiguration(const AQString &configuration);

   void build();
   void clean();
   void run();

private:
   void loadMakefile();

   vector<AQString> m_files;
   AQString m_projectPath;
   AQString m_configuration;
   AQString m_projectName;
};
