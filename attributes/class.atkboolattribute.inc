<?php
  /**
   * The atkBoolAttribute class represents an attribute of a node
   * that can either be true or false.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @version $Revision$
   *
   * $Id$   
   *
   */
  class atkBoolAttribute extends atkAttribute
  {
    var $m_onvalue = 1;
    var $m_offvalue = 0;
    
    /**
     * Constructor
     * @param String $name Name of the attribute
     * @param Flags $flags Flags for this attribute
     * @param Mixed $onvalue Value set when box is checked (default: 1)
     * @param Mixed $offvalue Value set when box is unchecked (default: 0)
     */
    function atkBoolAttribute($name, $flags=0, $onvalue=1, $offvalue=0)
    {
      //Setting AF_OBLIGATORY has no use, so prevent setting it.
      if (($flags & AF_OBLIGATORY) == AF_OBLIGATORY) $flags &= (~ AF_OBLIGATORY);          
      
      $this->m_onvalue = $onvalue;
      $this->m_offvalue = $offvalue;
            
     // Call base class constructor. Size of boolean value is always 1.
      $this->atkAttribute($name,$flags, 1); 
    }

    /**
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value.
     * @param $record Array with fields
     * @return piece of html code with a checkbox
     */
    function edit($record="", $fieldprefix="")
    {
      $checked = "";
      if ($record[$this->fieldName()]==$this->m_onvalue) $checked="checked";
      return '<input type="checkbox" id="'.$fieldprefix.$this->formName().'" name="'.$fieldprefix.$this->formName().'" value="'.$this->m_onvalue.'" '.$checked.'>';
    }
    

    /**
     * Get the value if it exits, otherwise return 0
     * @param $rec Array with values
     * @return String with slashes
     */
    function value2db($rec)
    {   
      return ($rec[$this->fieldName()]==$this->m_onvalue ? $this->m_onvalue: $this->m_offvalue);      
    }


    /**
     * Returns a piece of html code that can be used in a form to search for values   
     * @param $record Array with fields
     * @return piece of html code with a checkbox
     */
    function search($record="")
    {
      $result = '<select name="atksearch['.$this->formName().']">';
      $result.= '<option value="">'.text('search_all');
      $result.= '<option value="'.$this->m_offvalue.'" ';
      if($record[$this->fieldName()]!=$this->m_onvalue && !empty($record)) $result.="selected";
      $result.= '>'.text('no');
      $result.= '<option value="'.$this->m_onvalue.'" ';
      if($record[$this->fieldName()]==$this->m_onvalue) $result.="selected";
      $result.= '>'.text('yes');
      $result.='</select>';
      return $result;
    }   
    
   /**
     * Creates an search condition for a given search value
     * @param $table the table name
     * @param $value the search value
     * @return a piece of the SQL where clause
     */
    function searchCondition(&$query, $table, $value, $searchmode)
    {
      $query->addSearchCondition($query->exactCondition($table.".".$this->fieldName(),escapeSQL($value)));
    }     
    

    /**
     * Returns a displayable string for this value.
     * @param $record Array with boolean field
     * @return yes or no
     */
    function display($record)
    {
	    // We display true (1) as 'Yes' and false as 'No'
	    if ($record[$this->fieldName()]==$this->m_onvalue) return text("yes");
	    return text("no");
    }
    
    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      // Possible values
      //"regexp","exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal"
      return array("exact");
    }

    function dbFieldType()
    {
      // TODO/FIXME: this must be dependend on onvalue/offvalue
      return "number";
    }
    
  }
?>
