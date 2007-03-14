<?php

  atkimport('atk.datatypes.atkdatatype');
  class atkString extends atkDataType
  {
    protected $m_string="";

    public function __construct($string)
    {
      $this->m_string = $string;
    }

    public function replace($search, $replace)
    {
      $this->m_string = str_replace($search,$replace,$this->m_string);
      return $this;
    }

    public function replaceMultiple(Array $searches,Array $replaces)
    {
      foreach ($searches as $key=>$search)
      {
        $this->replace($search,$replaces[$key]);
      }
      return $this;
    }

    public function getString()
    {
      return $this->m_string;
    }

    public function dumpString($txt="")
    {
      atk_var_dump($this->m_string,$txt);
      return $this;
    }

    /**
     * To string. Returns the string representation for this object
     * which is ofcourse the internal string.
     *
     * @return string internal string
     */
    public function __toString()
    {
      return $this->m_string;
    }

    public function parse($data)
    {
      $this->m_string = atknew('atk.utils.atkstringparser',$this->m_string)->parse($data);
      return $this;
    }
  }

?>
