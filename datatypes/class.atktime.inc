<?php

  atkimport('atk.datatypes.atkdatatype');
  class atkTime extends atkDataType
  {
    /**
     * @var atkString The format to display the time with
     *                defaults to [hours]:[minutes]:[seconds]
     */
    private $m_format="";

    /**
     * @var int The time in UNIX timestamp format
     */
    private $m_time=0;

    public function __construct($time=0)
    {
      $this->setFormat('[hours]:[minutes]:[seconds]');
      if (is_array($time))
      {
        $this->m_time = gmmktime($time['hours'],$time['minutes'],$time['seconds'],1,1,1970);
      }
      else if (is_numeric($time)) $this->m_time = $time;
      else atkerror('Unknown time format! Called with: '."new atkTime($time)");
      atk_var_dump($this->getTotalSeconds(),"<h1>total seconds for atktime($time)</h1>");
    }

    public function getTimeArray($leadingzero=false)
    {
      return array('hours'=>  $this->getHours($leadingzero),
                   'minutes'=>$this->getMinutes($leadingzero),
                   'seconds'=>$this->getSeconds($leadingzero));
    }

    public function getHours($leadingzero=false)
    {
      $hours = ($this->m_time/3600);
      if ($leadingzero && $hours<10) $hours="0$hours";
      return $hours;
    }

    public function getMinutes($leadingzero=false)
    {
      $minutes = (($this->m_time-($this->getHours()*3600))/60);
      if ($leadingzero && $minutes<10) $minutes="0$minutes";
      return $minutes;
    }

    public function getTotalMinutes()
    {
      return (int)($this->m_time/60);
    }

    public function getSeconds($leadingzero=false)
    {
      $seconds = ($this->m_time-($this->getHours()*3600) - ($this->getMinutes()*60));
      if ($leadingzero && $seconds<10) $seconds="0$seconds";
      return $seconds;
    }

    public function getTotalSeconds()
    {
      return (int)$this->m_time;
    }

    public function isLesserThan(atkTime $compare)
    {
      return ($this->m_time<$compare->getTotalSeconds());
    }

    public function isGreaterThan(atkTime $compare)
    {
      atkdebug("atkTime::isGreaterThan() ($this->m_time>".$compare->getTotalSeconds().")");
      return ($this->m_time>$compare->getTotalSeconds());
    }

    public function isEqualTo(atkTime $compare)
    {
      return ($this->m_time===$compare->getTotalSeconds());
    }

    public function getDiff(atkTime $compare)
    {
      return (new atkTime((int)($this->m_time-$compare->getTotalSeconds())));
    }

    public function isNegative()
    {
      return ($this->m_time>0);
    }

    public function isNul()
    {
      return ($this->m_time===0);
    }

    /**
     * Enter description here...
     *
     * @param atkTime $compare
     * @return atkTime
     */
    public function add(atkTime $compare)
    {
      $this->m_time+=$compare->getTotalSeconds();
      return $this;
    }

    /**
     * Enter description here...
     *
     * @param atkTime $compare
     * @return atkTime
     */
    public function subtract(atkTime $compare)
    {
      $this->m_time-=$compare->getTotalSeconds();
      return $this;
    }

    /**
     * Enter description here...
     *
     * @param unknown_type $format
     * @return atkTime
     */
    public function setFormat($format)
    {
      $this->m_format = atknew('atk.datatypes.atkstring',$format);
      return $this;
    }

    public function getFormatted()
    {
      return $this->m_format->parse($this->getTimeArray())->getString();
    }

    /**
     * Enter description here...
     *
     * @param unknown_type $txt
     * @return atkTime
     */
    public function dump($txt)
    {
      atk_var_dump($this->getFormatted(),$txt);
      return $this;
    }
  }

  /**
   * Create a new atkTime instance and return it.
   *
   * @param mixed $time
   * @return atkTime
   */
  function new_atkTime($time)
  {
    return new atkTime($time);
  }

?>