<?php
/**
 * 
 * @authors Kees van Dieren <kees@ibuildings.nl>
 *          Ivo Jansch <ivo@ibuildings.nl>
 * @version $Revision$
 * $Id$
 * @copyright Ibuildings.nl BV
 * @package atk
 * @subpackage filters
 */

  usefilter("atkfilter");

  class atkGroupByFilter extends atkFilter
  {	
    /**
    * This variable is parsed as part of class classname
    *
    * @access private
    * @var vartype varname
    */
    var $m_groupbystmt;

    /**
    * description
    *
    * @return void
    */
    function atkGroupByFilter($name, $groupbystmt, $flags=0)
    {	
      $this->m_groupbystmt = $groupbystmt;
      $this->atkFilter($name, $flags);
    }

    /**
    * description
    *
    * @return void
    */
    function addToQuery(&$query)
    {	    
      $query->addGroupBy($this->m_groupbystmt);
    }
  }

?>