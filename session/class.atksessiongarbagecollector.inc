<?php
/**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage session
   * 
   * @author Marc Veldman
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

/**
   * 
   * @internal atkSessionGarbageCollector handles access to session variables and supports
   * timers to expire selected session variables.
   * atkSessionGarbageCollector provides access to $_SESSION[ $conifg_identifier ] through
   * the setVar and getVar method.
   * From the point of view of atkSessionGarbageCollector, the session variables are organized
   * like a tree:
   * 
   * Root +
   *      |
   *      +-Parent1-+
   *      |         |
   *      |         +-Child1--+
   *      |         |         |
   *      |         |         +-Grandchild1
   *      |         |
   *      |         +-Child2
   *      |
   *      +-Parent2-+
   *      |         |
   *      |         +-Child3--+
   *      |         |         |
   *      |         |         +-Grandchild2
   *      |         |
   *      |         +-Child4
   * 
   * Basic usage:
   * 
   * When you want to get the value of Grandchild2, use: $value = $atkSessionGarbageCollector->getVar('Parent2', 'Child3', 'Grandchild2');
   * When you want to set the value of Child2 use: $atkSessionGarbageCollector->setVar('Parent2','Child2', $value);
   * 
   * The depth of the tree is only limited by the PHP implementation.
   * 
   * Timers:
   * It is possible to put a timeout on session variables by using the setTimeOut method.
   * 
   * When you want Child3 to expire after at least 10 seconds, use:
   * $atkSessionGarbageCollector->setTimeOut('Parent2','Child3', 10);
   *      
   * The timers will be checked and variables removed with every run of
   * $atkSessionGarbageCollector->expiretimers()
   * The best place to call this function is probably in the constructor
   * or destructor of atkesssionmanager.
   */

/**
  * Seperator between namespace, variable and subvar in varid.
  */
define('SESS_VAR_SEPERATOR',':');

/**
  * Var/subvar name wildcard
  */
define('SESS_VAR_WILDCARD','*');

class atkSessionGarbageCollector
{
  /**
   * (Reference to) session data.
   * 
   * @access private
   * @var array
   */
  var $s_data; //

  /**
   * List of variables to expire.
   * @access private
   * @var array()
   *
   */
  var $s_expiretimers = null;
  /**
   * Constructor.
   * Set the default expiretimers from the config_session_cache_expire from config.inc.php
   *
   * @return atkSessionGarbageCollector
   */
  function atkSessionGarbageCollector()
  {
    $config_identifier = atkconfig("identifier");
    if (empty($config_identifier)) $config_identifier = "default";
    $this->s_data = &$_SESSION[$config_identifier];
    $this->s_expiretimers = atkconfig('session_var_expiry',array('stack:*'=>(atkconfig("session_cache_expire") * 60)));
  }

  /**
   * Check all timers, and remove the variable if it has not been accessed
   * for more than its timeout value.
   *
   */
  function expireTimers()
  {
    if (!is_array($this->s_data["__timer"]))
    {
      return false;
    }
    foreach ($this->s_data["__timer"] AS $varid => $last_access)
    {
      $vars = split(SESS_VAR_SEPERATOR,substr($varid,1));
      $check_id = $varid;
      $var_max_time = $this->_getTimeOutByID($varid);
      if ($var_max_time == false)
      {
        //No timeout for this variable, check parent.
        $parentid = $this->_getVarParent($vars);
        $var_max_time = $this->_getTimeOutByID($parentid);
      }
      //Only try to reset if we have found a timer for this variable.
      if ($var_max_time != false)
      {
        if((time() - $last_access) > $var_max_time)
        {
          $argtext = '"' . join('", "',$vars) .'"';
          eval('$this->unsetVar('.$argtext.');');
          atkwarning("Session Variable {$varid} expired");
          unset($this->s_data["__timer"][$varid]);
        }
      }
    }
  }

  /**
   * Calculate the variable name.
   * 
   * @param array list of names of the variables' 'ancestors'.
   * @return String variable name. 
   */
  function _getVarName($arg_list)
  {
    $varname = 's_data';
    foreach ($arg_list as $arg)
    {
      if (!is_string($arg))
      {
        atk_var_dump($arg_list, "ARG_LIST");
        atkerror('Invalid stack var name');
      }
      $varname .= '["'.$arg.'"]';
    }
    return $varname;
  }

  /**
     * Retrieve a session variable.
     *
     * Retrieve a 
     * Get a variable from the session.
     * @param list of names of the variables' 'ancestors'.
     * @return Mixed.
     */
  function getVar()
  {
    //If we don't provide at least one argument, complain.
    if (func_num_args() < 1)
    {
      atkerror("Called atkSessionGarbageCollector::getVar without arguments.");
      return null;
    }
    $this->_updateTimer(func_get_args());
    $varname = $this->_getVarName(func_get_args());
    $res = eval("return \$this->{$varname};");
    return $res;
  }

  /**
   * Set a session variable.
   * @access public
   * @param list of names of the variables' 'ancestors', followed by the value.
   * @return boolean. (true for succes, false for failure.)
   *
   */
  function setVar()
  {
    $arg_list = func_get_args();
    $arg_c = func_num_args();
    //If there is more than 1 argument, assume that the last one is the value.
    if ($arg_c > 1)
    {
      $value = array_pop($arg_list);
    }
    //If we have exactly one, assume we want to unset this one.
    elseif ($arg_c == 1)
    {
      $value = null;
    }
    //If we have less than one, complain, do nothing and exit.
    else
    {
      atkerror("Called atkSessionGarbageCollector::setVar without arguments.");
      return false;
    }
    //If the value is not empty, set the variable, otherwise unset it.
    $varname = $this->_getVarName($arg_list);
    if (!empty($value))
    {
      $res = eval('$var =& $this->'.$varname.";");
      $var = $value;
      $this->_updateTimer($arg_list);
    }
    else
    {
      $res = eval('unset($this->'.$varname.");");
    }
    return true;
  }

  /**
   * Set the timeout for a session variable.
   * @param list of names of the variables' 'ancestors'.
   * @return boolean. (true for succes, false for failure.)
   */
  function setTimeOut()
  {
    $arg_list = func_get_args();
    $arg_c = func_num_args();
    //If there is more than 1 argument, assume that the last one is the value.
    if ($arg_c > 1)
    {
      $timeout = array_pop($arg_list);
    }
    //If we have one or less, complain and exit.
    else
    {
      atkerror("Called atkSessionGarbageCollector::setTimeOut with no or only one argument.");
      return false;
    }
    $varid = $this->_getVarID($arg_list);
    if(intval($timeout) > 0)
    {
      $this->s_expiretimers[$varid] = $timeout;
    }
    else
    {
      $this->unsetTimeOut($varid);
    }
    return true;
  }
  /**
   * Set the timeout for a session variable.
   * @param list of names of the variables' 'ancestors'.
   */
  function unsetTimeOut()
  {
    $varid = $this->_getVarID(func_get_args());
    if (isset($this->s_expiretimers[$varid]))
    {
      unset($this->s_expiretimers[$varid]);
    }
  }
  /**
   * Get the timeout for a session variable.
   * @param list of names of the variables' 'ancestors'.
   * @return  integer
   */
  function getTimeOut()
  {
    $varid = $this->_getVarID(func_get_args());
    if(isset($this->s_expiretimers[$varid]))
    {
      return $this->s_expiretimers[$varid];
    }
    else
    {
      return false;
    }
  }
  /**
   * Unset a session variable.
   * @param list of names of the variables' 'ancestors'.
   * @access public
   *
   */
  function unsetVar()
  {
    $varname = $this->_getVarName(func_get_args());
    $eval = '$this->'.$varname;
    $res = eval('unset($this->'.$varname.");");
  }

  /**
   * Get the timeout value of a session variable based on its variable ID.
   *
   * @param String $varid
   * @return unknown
   * @access private
   */
  function _getTimeOutByID($varid)
  {
    if (isset($this->s_expiretimers[$varid]))
    {
      return $this->s_expiretimers[$varid];
    }
    else
    {
      false;
    }
  }

  /**
   * Get the ID of a session variable.
   *
   * @param list of names of the variables' 'ancestors'.
   * @return String
   * @access private
   */
  function _getVarID($arg_list)
  {
    return SESS_VAR_SEPERATOR . implode(SESS_VAR_SEPERATOR, $arg_list);
  }

  /**
   * Get the ID of the 'parent' of a session variable.
   *
   * @param array $arg_list : names of the variables.
   * @return String
   * @access private
   */
  function _getVarParent($arg_list)
  {
    $parent_id = SESS_VAR_SEPERATOR.SESS_VAR_WILDCARD;
    array_pop($arg_list);
    if (!empty($arg_list))
    {
      $parent_id = SESS_VAR_SEPERATOR . implode(SESS_VAR_SEPERATOR,$arg_list) . $parent_id;
    }
    return $parent_id;
  }

  /**
   * Update the timer for a given variable.
   *
   * @param list of names of the variables' 'ancestors'.
   * @access private
   */
  function _updateTimer($arg_list)
  {
    $varid = $this->_getVarID($arg_list);
    $parentid = $this->_getVarParent($arg_list);
    if (key_exists($varid, $this->s_expiretimers) || key_exists($parentid, $this->s_expiretimers))
    {
      $this->s_data['__timer'][$this->_getVarID($arg_list)] = time();
    }
  }
}

?>
