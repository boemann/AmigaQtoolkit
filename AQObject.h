#ifndef AQOBJECT_H
#define AQOBJECT_H

#include <vector.h>

#include <AQString.h>

class AQObject;

class ConnectionBase
{
public:
   ConnectionBase(const AQString &signalName);
   void registerConnection(AQObject *sender, ConnectionBase *);

   virtual void invoke() = 0;
   virtual void invoke(int arg) = 0;
   virtual void invoke(AQObject *arg) = 0;
   AQString m_signalName;
};

template <class Receiver>
class Connect : public ConnectionBase
{
public:
   Connect(const AQString &signalName, Receiver *r, void (Receiver::*method)())
      : ConnectionBase(signalName)
      , m_receiver(r)
      , m_methodVoid(method)
      , m_methodInt(nullptr)
      , m_methodObj(nullptr)
   {
   }
   Connect(const AQString &signalName, Receiver *r, void (Receiver::*method)(int))
      : ConnectionBase(signalName)
      , m_receiver(r)
      , m_methodVoid(nullptr)
      , m_methodInt(method)
      , m_methodObj(nullptr)
   {
   }
   Connect(const AQString &signalName, Receiver *r, void (Receiver::*method)(AQObject *))
      : ConnectionBase(signalName)
      , m_receiver(r)
      , m_methodVoid(nullptr)
      , m_methodInt(nullptr)
      , m_methodObj(method)
   {
   }

   Connect(AQObject *sender, const AQString &signalName, Receiver *r, void (Receiver::*method)())
      : ConnectionBase(signalName)
   {
      registerConnection(sender, new Connect(signalName, r, method));
   }

   Connect(AQObject *sender, const AQString &signalName, Receiver *r, void (Receiver::*method)(int))
      : ConnectionBase(signalName)
   {
      registerConnection(sender, new Connect(signalName, r, method));
   }

   Connect(AQObject *sender, const AQString &signalName, Receiver *r, void (Receiver::*method)(AQObject *))
      : ConnectionBase(signalName)
   {
      registerConnection(sender, new Connect(signalName, r, method));
   }

   void invoke(int arg)
   {
      if (m_methodInt != nullptr)
	      ((m_receiver)->*(m_methodInt))(arg);

      invoke();
   }

   void invoke(AQObject *arg)
   {
      if (m_methodObj != nullptr)
         ((m_receiver)->*(m_methodObj))(arg);

      invoke();
   }
 
   void invoke()
   {
      if (m_methodVoid != nullptr)
         ((m_receiver)->*(m_methodVoid))();
   }

private:
   Receiver *m_receiver;
   void (Receiver::*m_methodVoid)();
   void (Receiver::*m_methodInt)(int);
   void (Receiver::*m_methodObj)(AQObject *);
};

class AQObject
{
public:
   AQObject(AQObject *parent = nullptr);
   virtual ~AQObject();

   void setParent(AQObject *p);

   void emit(const AQString &signalName, int arg);
   void emit(const AQString &signalName, AQObject *arg);
   void emit(const AQString &signalName);

friend class ConnectionBase;

private:
   void registerConnection(ConnectionBase *cb);
   
   vector<ConnectionBase *> m_connections;

   AQObject *m_parent;
   vector<AQObject *> m_children;
};

#endif