<?php

  /**
   * The atkListAttribute class represents an attribute of a node
   * that has a selectbox to select from predefined values.
   *
   * @author Ivo Jansch (ivo@achievo.org)
   * @version $Revision$
   *
   * $Id$
   *
   */
  class atkListAttribute extends atkAttribute
  {
    /**
     * Array with options for Listbox
     */
    var $m_options = Array();

    /**
     * Array with values for Listbox
     */
    var $m_values = Array();

    /**
     * Array for fast lookup of what value belongs to what option.
     */
    var $m_lookup = Array();

    /**
     * # rows to show (multiselect listbox).
     */
    var $m_listrows = 1;     


    /**
     * Constructor.
     *
     * Warning: very old versions of this attribute supported passing the
     * parameters in a different order: $name, $flags, $optionArray.
     * This order used to be supported even when the new order was
     * implemented, but it has now been removed. Keep this in mind
     * when upgrading from a very old ATK version (pre ATK4).
     *
     * @param $name Name of the attribute
     * @param $optionArray Array with options
     * @param $valueArray Array with values. If you don't use this parameter,
     *                    values are assumed to be the same as the options.
     * @param $flags Flags for this attribute
     * @param $size  Size of the attribute.
     */
    function atkListAttribute($name, $optionArray, $valueArray="", $flags=0, $size=0, $listrows=1)
    {
      if (!is_array($valueArray) || count($valueArray)==0)
      {
        $valueArray = $optionArray;
      }

      // If all values are numeric, we can use a numeric field to store the selected
      // value.
      $this->m_dbfieldtype = "number";
      for ($i=0, $_i=count($valueArray); $i<$_i && $this->m_dbfieldtype=="number"; $i++)
      {
        if (!is_numeric($valueArray[$i])) $this->m_dbfieldtype="string";
        // if one of the values is not a number, the fieldtype must be string, and
        // the loop is stopped.
      }

      // If no size is specified, the max size we have is equal to the biggest value.
      if ($size==0)
      {
        for ($i=0, $_i=count($valueArray); $i<$_i; $i++)
        {
          $size = max($size, strlen($valueArray[$i]));
        }
      }

      // Multiple rows ?
      $this->m_listrows = $listrows;

      $this->atkAttribute($name, $flags, $size); // base class constructor
      $this->m_options  = $optionArray;
      $this->m_values   = $valueArray;


      for ($i=0, $_i=count($optionArray); $i<$_i; $i++)
      {
        $this->m_lookup[$this->m_values[$i]] = $optionArray[$i];
      }
    }


    /**
     * Display's text version of Record
     * @param $record
     * @return text string of $record
     */
    function display($record)
    {
      global $atkaction;

      if ($this->m_listrows > 1)
      {
        $multiValues = $record[$this->fieldName()];

        for ($i=0;$i<=count($multiValues)-1;$i++)
        {
          $retVal .= text($this->m_lookup[$multiValues[$i]], $this->m_ownerInstance->m_type).", ";
        }
        return substr($retVal,0,-2);

      }
      else
      {
        return text($this->m_lookup[$record[$this->fieldName()]], $this->m_ownerInstance->m_type);
      }
    }

    /**
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value.
     * @param $record Array with fields
     * @return piece of html code with a checkbox
     */
    function edit($record="", $fieldprefix="")
    {
      if ($this->m_listrows > 1)
      {
        // Add extra param
        $multiSelect = " MULTIPLE SIZE='".$this->m_listrows."'";

        // Add [] to the fieldname so an array is posted
        $namePostFix = "[]";
      }

      $result = '<select name="'.$fieldprefix.$this->formName().$namePostFix.'" '.$multiSelect.'>';
      if ($this->hasFlag(AF_OBLIGATORY)==true && atkconfig('obligatory_no_default'))
      {
        $result.= '<option value="">'.text('select_none');
      }
      
      for ($i=0;$i<count($this->m_values);$i++)
      {
      	
        if ($this->m_values[$i] == $record[$this->fieldName()] && $record[$this->fieldName()]!="")
        {
          $sel = "selected";
        }
        else
        {
          $sel = "";
        }
        $result.= '<option value="'.$this->m_values[$i].'" '.$sel.'>'.text($this->m_options[$i], $this->m_ownerInstance->m_type);
      }
      $result.='</select>';
      return $result;
    }

    /**
     * Returns a piece of html code that can be used in a form to search values
     * @param $record Array with fields
     * @return piece of html code with a checkbox
     */
    function search($record="", $extended=false)
    {
      // todo: configurable rows
      $result = '<select ';
      if ($extended)
      {
        $result.='multiple size="'.min(5,count($this->m_values)+1).'"';
      }
      $result.='name="atksearch['.$this->formName().'][]">';
      $result.= '<option value="">'.text('search_all');

      for ($i=0,$_i=count($this->m_values);$i<$_i;$i++)
      {                
        if (atk_in_array(((string)$this->m_values[$i]),$record[$this->fieldName()], true) && $record[$this->fieldName()]!=="")
        {
          $sel = "selected";
        }
        else
        {
          $sel = "";
        }
        $result.= '<option value="'.$this->m_values[$i].'" '.$sel.'>'.text($this->m_options[$i], $this->m_ownerInstance->m_type);
      }
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
      // We only support 'exact' matches.
      // But you can select more than one value, which we search using the IN() statement,
      // which should work in any ansi compatible database.
      if (is_array($value) && count($value)>0 && $value[0]!="") // This last condition is for when the user selected the 'search all' option, in which case, we don't add conditions at all.
      {
        if (count($value)==1) // exactly one value
        {
          $query->addSearchCondition($query->exactCondition($table.".".$this->fieldName(),escapeSQL($value[0])));
        }
        else // search for more values using IN()
        {
          $query->addSearchCondition($table.".".$this->fieldName()." IN ('".implode("','",$value)."')");
        }
      }
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
      // Fieldtype was determined in the constructor.
      return $this->m_dbfieldtype;
    }


    function value2db($rec)
    {
      if(is_array($rec[$this->fieldName()]) && count($rec[$this->fieldName()]>=1) && $this->m_listrows > 1)
      {
        return implode("|",escapeSQL($rec[$this->fieldName()]));
      }
      else
      {
        return escapeSQL($rec[$this->fieldName()]);
      }
    }

    function db2value($rec)
    {
      // if this is a multirow item explode the items
      if ($this->m_listrows > 1)
      {
        $dummy = explode("|", stripslashes($rec[$this->fieldName()]));
      }
      else
      {
        $dummy = stripslashes($rec[$this->fieldName()]);
      }

      return $dummy;
    }


  }
?>