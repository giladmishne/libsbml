/**
 * @file    Validator.h
 * @brief   Base class for SBML Validators
 * @author  Ben Bornstein
 *
 * $Id$
 * $Source$
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright 2005-2007 California Institute of Technology.
 * Copyright 2002-2005 California Institute of Technology and
 *                     Japan Science and Technology Corporation.
 * 
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution and
 * also available online as http://sbml.org/software/libsbml/license.html
 *----------------------------------------------------------------------- -->*/

#ifndef Validator_h
#define Validator_h


#ifdef __cplusplus


#include <list>
#include <string>

#include <sbml/SBMLError.h>


class VConstraint;
class ValidatorConstraints;
class SBMLDocument;


class Validator
{
public:

  Validator ( SBMLError::SBMLCategory category = SBMLError::SBML );

  virtual ~Validator ();

  /**
   * Initializes this Validator with a set of Constraints.
   *
   * When creating a subclass of Validator, override this method to add
   * your own Constraints.
   */
  virtual void init () = 0;

  /**
   * Adds the given Contraint to this validator.
   */
  void addConstraint (VConstraint* c);

  /**
   * Clears the Validator's list of messages.
   *
   * If you are validating multiple SBML documents with the same Validator,
   * call this method after you have processed the list of messages from
   * the last Validation run and before validating the next document.
   */
  void clearMessages ();

  /**
   * @return the category covered by this Validator.  A category is a
   * string, similiar in spirit to an XML namespace, which partitions error
   * messages to prevent id conflicts.  Example categories include:
   *
   *   http://sbml.org/validator/consistency
   *   http://sbml.org/validator/consistency/units
   *   http://sbml.org/validator/compatibility/L1
   */
  const SBMLError::SBMLCategory getCategory () const;

  /**
   * @return a list of messages logged during validation.
   */
  const std::list<SBMLError>& getMessages () const;

  /**
   * Adds the given message to this list of Validators messages.
   */
  void logMessage (const SBMLError& msg);

  /**
   * Validates the given SBMLDocument.  Error messages logged during
   * validation may be retrieved via <code>getMessages()</code>.
   *
   * @return the number of validation errors that occurred.
   */
  unsigned int validate (const SBMLDocument& d);

  /**
   * Validates the given SBMLDocument.  Error messages logged during
   * validation may be retrieved via <code>getMessages()</code>.
   *
   * @return the number of validation errors that occurred.
   */
  unsigned int validate (const std::string& filename);


protected:
  /** @cond doxygen-libsbml-internal */


  ValidatorConstraints*   mConstraints;
  std::list<SBMLError>    mMessages;
  SBMLError::SBMLCategory mCategory;


  friend class ValidatingVisitor;

  /** @endcond doxygen-libsbml-internal */
};


#endif  /* __cplusplus */
#endif  /* Validator_h */
