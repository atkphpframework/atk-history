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
          $result.= '<select name="'.$fieldprefix.$this->formName().'[hours]">';
          for ($h=0;$h<=$this->m_maxtime_min/60;)
          { 
            $result .= '<option value="'.$h.'" ';         
            if ($record[$this->fieldName()]['hours']==$h) $result.= "selected";
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
            if ($record[$this->fieldName()]['minutes']==$m) $result.= "selected";
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
        $result = '<input type="text" name="'.$fieldprefix.$this->formName().
                '" value="'.htmlspecialchars($record[$this->fieldName()]).'"'.
                ($this->m_size > 0 ? ' size="'.$this->m_size.'"' : '').
                ($this->m_maxsize > 0 ? ' maxlength="'.$this->m_maxsize.'"' : '').'>';        
      }
      return $result;
    }
    
    function hide($record="", $fieldprefix="")
    {
      if(!$this->hasFlag(AF_DURATION_STRING))
      {
        $result = '<input type="hidden" name="'.$fieldprefix.$this->formName().'[hours]" value="'.$record[$this->fieldName()]["hours"].'">';
        $result.= '<input type="hidden" name="'.$fieldprefix.$this->formName().'[minutes]" value="'.$record[$this->fieldName()]["minutes"].'">';
      }
      else 
      {
        $result.='<input type="hidden"  name="'.$fieldprefix.$this->formName().'" value="'.$record[$this->fieldName()].'">';
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
     * Convert Time Array to minutes
     * @param $rec Array with 2 fields (hours, minutes)
     * @return integer indicating the number of minutes
     */
    function value2db($rec)
    {
      return $this->_value2minutes($rec[$this->fieldName()]);
    }

    /**
     * Convert time to Array
     * @param $rec contains the time in minute format
     * @return Array with 2 fields (hours, minutes)
     */
    function db2value($rec)
    {
      return $rec[$this->fieldName()];
    }

    /**
     * Function display's the time
     * @param $record Array with time
     * @return String with YYYY-MM-DD
     */
    function display($rec)
    {
      return $this->_minutes2value($rec[$this->fieldName()]);
    }
    
    /**
     * Check if the attribute is empty
     * @return true if it's empty
     */
    function isEmpty($postvars)
    {
      if(!$this->hasFlag(AF_DURATION_STRING))
      {
        return ($postvars[$this->fieldName()]["hours"]==0&&$postvars[$this->fieldName()]["minutes"]==0);
      }
      else
      {
        
      } return ($postvars[$this->fieldName()]=="");
    }
    
    function sum($rec1, $rec2)
    { 
      if(!$this->hasFlag(AF_DURATION_STRING))
      {
           
        if (!is_array($rec1))
        {   
          $a=Array("hours"=>0,"minutes"=>0);       
        }
        else
        {        
          $a = $rec1[$this->fieldName()];
        }      
        
        $b = $rec2[$this->fieldName()];              
        
        $total = ($a["hours"]*60)+$a["minutes"]+($b["hours"]*60)+$b["minutes"];      
        return Array($this->fieldName()=>Array("hours"=>(floor($total/60)),"minutes"=>$total%60));
      }
      else 
      {
        $total = $rec1[$this->fieldName()]+$rec2[$this->fieldName()];
        return Array($this->fieldName()=>$total);        
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
      return "number";
    }
    
    function _value2minutes($value)
    {
      if(!$this->hasFlag(AF_DURATION_STRING)) 
      {
        return $value["hours"]*60+$value["minutes"];
      }
      else
      {
        if(strpos($value,':')===false)
        {
          $tmp = explode('.',$value);
          return $tmp[0]*60+$tmp[1]*(60/100);
        }
        else
        {
          $tmp = explode(':',$value);
          return $tmp[0]*60+$tmp[1];
        }
      }
    }
    
    function _minutes2value($minutes)
    {
      if(!$this->hasFlag(AF_DURATION_STRING))
      {
        $hours = floor($minutes/60);
        return Array("hours"=>$hours,
                     "minutes"=>$minutes-($hours*60)
                     );
      }
      else
      {
        if(atkconfig("durationformat",0)==DURATIONFORMAT_DECIMAL)
        {
          $hours=floor($minutes/60);
          $min = $minutes-($hours*60);
          return sprintf("%02d",$hours).".".sprintf("%02d",$minutes*(60/100));
        }
        elseif(atkconfig("durationformat",0)==DURATIONFORMAT_TIME)
        {
          atkdebug("Minutes: ".$minutes);
          atk_var_dump($minutes);
          $hours = floor($minutes/60);
          $min = $minutes-($hours*60);
          return sprintf("%02d",$hours).":".sprintf("%02d",$min);
           
        }
      }
    }

    function _str_contains($haystack, $needle, $ignoreCase = false) 
    {
      if ($ignoreCase) 
      {
        $haystack = strtolower($haystack);
        $needle  = strtolower($needle);
      }
      $needlePos = strpos($haystack, $needle);
      return ($needlePos === false ? false : ($needlePos+1));
    }
  }
?>