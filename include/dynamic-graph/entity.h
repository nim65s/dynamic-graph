// -*- mode: c++ -*-
// Copyright 2010, François Bleibel, Thomas Moulard, Olivier Stasse,
// JRL, CNRS/AIST.
//

#ifndef DYNAMIC_GRAPH_ENTITY_H
#define DYNAMIC_GRAPH_ENTITY_H
#include <iosfwd>
#include <map>
#include <sstream>
#include <string>

#include <boost/noncopyable.hpp>

#include <dynamic-graph/dynamic-graph-api.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/fwd.hh>
#include <dynamic-graph/logger.h>
#include <dynamic-graph/signal-array.h>
#include <dynamic-graph/signal-base.h>

/// \brief Helper macro for entity declaration.
///
/// This macro should be called in the declaration of all entities.
/// Example:
/// <code>
/// class A : public dynamicgraph::entity
/// {
///   DYNAMIC_GRAPH_ENTITY_DECL();
///
///   public:
//      // your class here
/// };
/// </code>
///
/// Caution: you *MUST* call DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN in the
/// associated source file to ensure that the attributes generated by
/// this macro are correctly initialized.
#define DYNAMIC_GRAPH_ENTITY_DECL()                                            \
public:                                                                        \
  virtual const std::string &getClassName() const { return CLASS_NAME; }       \
  static const std::string CLASS_NAME

namespace dynamicgraph {
/// \ingroup dgraph
///
/// \brief This class represents an entity, i.e. a generic
/// computational unit that provides input and output signals.
///
/// These signals link the entities together to form a complete
/// computation graph.  To declare a new entity, please see the
/// DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN macro in factory.h.
class DYNAMIC_GRAPH_DLLAPI Entity : private boost::noncopyable {
public:
  typedef std::map<std::string, SignalBase<int> *> SignalMap;
  typedef std::map<const std::string, command::Command *> CommandMap_t;

  explicit Entity(const std::string &name);
  virtual ~Entity();

  const std::string &getName() const { return name; }
  virtual const std::string &getClassName() const {
    static std::string ret("Entity");
    return ret;
  }
  virtual std::string getDocString() const;
  bool hasSignal(const std::string &signame) const;
  SignalBase<int> &getSignal(const std::string &signalName);
  const SignalBase<int> &getSignal(const std::string &signalName) const;
  std::ostream &displaySignalList(std::ostream &os) const;
  virtual std::ostream &writeGraph(std::ostream &os) const;
  virtual std::ostream &writeCompletionList(std::ostream &os) const;

  virtual void display(std::ostream &os) const;

  virtual SignalBase<int> *test() { return 0; }

  virtual void test2(SignalBase<int> *) { return; }

  const std::string &getCommandList() const;
  CommandMap_t getNewStyleCommandMap();
  command::Command *getNewStyleCommand(const std::string &cmdName);

  SignalMap getSignalMap() const;

  /// \name Logger related methods
  /// \{

  Logger &logger() { return logger_; };
  Logger const &logger() const { return logger_; };

  /// \brief Send messages \c msg with level \c t.
  /// Add string file and line to message.
  void sendMsg(const std::string &msg, MsgType t = MSG_TYPE_INFO,
               const char *file = "", int line = 0);

  /// \brief Specify the verbosity level of the logger.
  void setLoggerVerbosityLevel(LoggerVerbosity lv) { logger_.setVerbosity(lv); }

  /// \brief Get the logger's verbosity level.
  LoggerVerbosity getLoggerVerbosityLevel() { return logger_.getVerbosity(); }

  /// \brief Set the time sample.
  bool setTimeSample(double t) { return logger_.setTimeSample(t); }

  /// \brief Get the time sample.
  double getTimeSample() { return logger_.getTimeSample(); }

  /// \brief Set the period of the stream period
  bool setStreamPrintPeriod(double t) {
    return logger_.setStreamPrintPeriod(t);
  }

  /// \brief Get the period of the stream period
  double getStreamPrintPeriod() { return logger_.getStreamPrintPeriod(); }

  /// \}

protected:
  void addCommand(const std::string &name, command::Command *command);

  void entityRegistration();
  void entityDeregistration();

  void signalRegistration(const SignalArray<int> &signals);
  void signalDeregistration(const std::string &name);

  std::string name;
  SignalMap signalMap;
  CommandMap_t commandMap;
  Logger logger_;
};

DYNAMIC_GRAPH_DLLAPI std::ostream &operator<<(std::ostream &os,
                                              const dynamicgraph::Entity &ent);
} // end of namespace dynamicgraph

#endif //! DYNAMIC_GRAPH_ENTITY_H
