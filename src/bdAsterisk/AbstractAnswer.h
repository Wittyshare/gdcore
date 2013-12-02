/*
 *
 *       Filename:  AbstractAnswer.h
 *
 *    Description:
 *
 *        Created:  07/31/2012 09:58:21 AM
 *
 *         Author:  Ben D. (), dbapps2@gmail.com
 *
 */

#ifndef ABSTRACT_ANSWER_H__
#define ABSTRACT_ANSWER_H__

#include <iostream>

class AbstractAnswer {

public:

  ~AbstractAnswer();

  virtual void parse() = 0;

  virtual void setAnswer(const std::string& msg) = 0;

};

#endif
