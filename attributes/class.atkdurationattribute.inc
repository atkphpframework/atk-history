<?php
  /**
   * The atkDurationAttribute is an attribute for entering a length of time.
   *
   * Display modes
   * -   (1.25)
   * -   (4:30)
   * @author  Ivo Jansch <ivo@achievo.org>
   * @version $Revision$
   *
   * $Id$
   *
   */
   
  /* flag(s) specific for atkDurationAttribute */
  define("AF_DURATION_STRING",              AF_SPECIFIC_1); // date must be entered as an english date string (strtotime)

  define("DURATIONFORMAT_TIME",0);
  define("DURATIONFORMAT_DECIMAL",1);
  
  class atkDurationAttribute extends atkAttribute
  {    
    /** member vars **/
    var $m_resolution_min;
    var $m_maxtime_min;
    
  
    /**
     * Default Constructor, sets up atkAttribute 
     *
     * @param $name
     * @param $resolution
     * @param $maxtime
     * @param $flags
     * @see atkAttribute
     */
    function atkDurationAttribute($name, $resolution="1m", $maxtime="10h", $flags=0, $size=0)
    {
      $this->atkAttribute($name,$flags, $size); // base class constructor
      $hms = substr($resolution,-1);
      $resolution = substr($resolution,0,-1);

      if (strtoupper($hms)=="H") $factor = 60;
      else $factor=1;      
     
      $this->m_resolution_min = $resolution*$factor;
      
      $hms = substr($maxtime,-1);
      $maxtime = substr($maxtime,0,-1);
      if (strtoupper($hms)=="H") $factor = 60;
      else $factor=1;      
      
      $this->m_maxtime_min = $maxtime*$factor;
    }
    
    function hide($record, $fieldprefix="")
    {
      // hide as a parseable string
      $record[$this->fieldName()] = $this->_minutes2string($record[$this->fieldName()]);
      return parent::hide($record, $fieldprefix);
    }

    /**   
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value. (hours, minutes and seconds will be a dropdownbox)
     * @params $record 
     * @return Piece a of HTML Code
     */
    function edit($record="", $fieldprefix="")
    {  
      if(!$this->hasFlag(AF_DURATION_STRING))
      {
        if ($this->m_maxtime_min>=60)
        {
          $curhours = $this->_getHourPart($record[$this->fieldName()]);
          $curminutes = $this->_getMinutePart($record[$this->fieldName()]);
          $result.= '<select name="'.$fieldprefix.$this->formName().'[hours]">';
          for ($h=0;$h<=$this->m_maxtime_min/60;)
          { 
            $result .= '<option value="'.$h.'" ';         
            if ($curhours==$h) $result.= "selected";
            $result .='>'.$h.' '.text('hours');
            if ($this->m_resolution_min<=60)
            {
              $h++;
            }
            else
            {
              $h = floor($h+$this->m_resolution_min/60);
            }
          }
        }
        $result.='</select>';
        
        if ($this->m_maxtime_min>=1&&$this->m_resolution_min<60)
        {
          $result.= '<select name="'.$fieldprefix.$this->formName().'[minutes]">';
          for ($m=0;$m<60||($this->m_maxtime_min<60 && $m<$this->m_maxtime_min);)
          { 
            $result .= '<option value="'.$m.'" ';
            if ($curminutes==$m) $result.= "selected";
            $result.='>'.$m.' '.text('minutes');
            if ($this->m_resolution_min<=1)
            {
              $m++;
            }
            else
            {
              $m = $m+$this->m_resolution_min;
            }
          }
        }
        $result.='</select>';   
      }
      else
      {
        $curval = ($record[$this->fieldName()]>0?$this->_minutes2string($record[$this->fieldName()]):"");
        $result = '<input type="text" name="'.$fieldprefix.$this->formName().
                '" value="'.$curval.'"'.
                ($this->m_size > 0 ? ' size="'.$this->m_size.'"' : '').'>';        
      }
      return $result;
    }        
    
    /**   
     * Returns a piece of html code that can be used in a form to search values.    
     * @params $record Array of records, with for each record 2 fields (hours, minutes)
     * @return Piece a of HTML Code
     */
    function search($record="")
    {
      return "&nbsp;"; // currently not searchable.
    }        

    /**
     * Function display's the time
     * @param $record Array with time
     * @return String with YYYY-MM-DD
     */
    function display($rec)
    {
      return $this->_minutes2string($rec[$this->fieldName()]);
    }        
    
    function getSearchModes()
    {      
      return array("exact");
    }
    
    function dbFieldType()
    {
      return "number";
    }
    
    function _string2minutes($value)
    {     
      if(strpos($value,':')===false)
      {
        // decimal format
        $tmp = explode('.',$value);
        if(strlen($tmp[1])==1) $tmp[1]=$tmp[1]*10;
        return $tmp[0]*60+$tmp[1]*(60/100);
      }
      else
      {
        // hh:mm format
        $tmp = explode(':',$value);
        return $tmp[0]*60+$tmp[1];
      }    
    }
    
    function _minutes2string($minutes)
    {
      if(atkconfig("durationformat",0)==DURATIONFORMAT_DECIMAL)
      {        
        $decimalvalue = $this->_getHourPart($minutes) + ($this->_getMinutePart($minutes)/60);
        return sprintf("%02.02f", $decimalvalue);
      }
      elseif(atkconfig("durationformat",0)==DURATIONFORMAT_TIME)
      {        
        return sprintf("%02d:%02d",$this->_getHourPart($minutes), $this->_getMinutePart($minutes));        
      }
    }   
    
    function _getHourPart($minutes)
    {
      return floor($minutes/60);
    }
    
    function _getMinutePart($minutes)
    {
      return ($minutes-(floor($minutes/60)*60));
    }
    
    /**
     * Fetch values
     * @param $rec Array with values
     * @return String without slashes
     */
    function fetchValue($rec)
    {
      if ($this->hasFlag(AF_DURATION_STRING))
      {
        return $this->_string2minutes($rec[$this->fieldName()]);       
      }
      else 
      {
        return $rec[$this->fieldName()]["hours"]*60 + $rec[$this->fieldName()]["minutes"];
      }
    }
    
  }
?>