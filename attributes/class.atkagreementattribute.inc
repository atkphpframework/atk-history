<?php
useattrib('atkboolattribute');

/**
 * The atkAgreementAttribute class represents an attribute of a node
 * that should always be true. The attribute will only validate if
 * the user has checked the agreement checkbox.
 *
 * @author Peter C. Verhage <peter@ibuildings.nl>
 * @version $Revision$
 */
class atkAgreementAttribute extends atkBoolAttribute
{
  /**
   * Constructor.
   *
   * @param String $name attribute name
   * @param int $flags attribute flags
   * @return atkAgreementAttribute
   */
  function atkAgreementAttribute($name, $flags=0)
  {
    $this->atkBoolAttribute($name, $flags);
    $this->addFlag(AF_OBLIGATORY);
  }
  
  /**
   * Check if value is empty.
   *
   * @param Array $postvars
   */
  function isEmpty($postvars)
  {
    return atkAttribute::isEmpty($postvars);
  }
}