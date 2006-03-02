<?

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage db
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */
  
  /**
   * Base include
   */
  atkimport("atk.db.atkmysqlquery");

  /**
   * SQL Builder for MSSQL databases. 
   *
   * @author Harrie Verveer <harrie@ibuildings.nl>
   * @package atk
   * @subpackage db
   *
   */
  class atkMssqlQuery extends atkMysqlQuery
  {
    /**
     * Generate an SQL searchcondition for a regular expression match.
     *
     * @param String $field The fieldname on which the regular expression
     *                      match will be performed.
     * @param String $value The regular expression to search for.
     * @param boolean $inverse Set to false (default) to perform a normal 
     *                         match. Set to true to generate a SQL string 
     *                         that searches for values dat do not match.
     * @todo fixme
     * @return String A SQL regexp expression.
     */ 
    function regexpCondition($field, $value, $inverse=false)
    {
      // void implementation
      return "";
    }
    
    /**
     * Generate an SQL searchcondition for a soundex match.
     *
     * @param String $field The fieldname on which the soundex match will 
     *                      be performed.
     * @param String $value The value to search for.
     * @param boolean $inverse Set to false (default) to perform a normal 
     *                         match. Set to true to generate a SQL string 
     *                         that searches for values dat do not match.
     * @todo fixme
     * @return String A SQL soundex expression.
     */ 
    function soundexCondition($field, $value, $inverse=false)
    {
      // void implementation
      return "";
    }
    
    /**
     * Prepare the query for a limit. 
     * @access private
     * @param String $query The SQL query that is being constructed.
     * @todo fixme
     */
    function _addLimiter(&$query)
    {
      // void implementation
    }
  }

?>