<?php
  /**
   *
   *
   * @author      : Ivo Jansch <ivo@achievo.org>
   * @version     : $Revision$
   *
   * $Id$
   *
   */

  userelation("atkmanytomanyrelation");

  class atkManyBoolRelation extends atkManyToManyRelation
  {

    /**
    * Constructor
    * @param $name
    * @param $destination
    * @param $flags
    */
    function atkManyBoolRelation($name, $link, $destination, $flags=0)
    {
      $this->atkManyToManyRelation($name, $link, $destination, $flags);
    }

    /**
     * Return a piece of html code to edit the attribute
     * @param $record Current record
     * @result piece of html code
     */
    function edit($record="", $fieldprefix="")
    {    
      $this->createDestination();
      $this->createLink();

      $selectedPk = array();          

      // first the selected records..
      for ($i=0;$i<count($record[$this->m_name]);$i++)
      {         
        $selectedPk[] = $this->m_destInstance->primaryKey($record[$this->m_name][$i][$this->remoteKey()]);
      }      

      // now select all records
      $recordset = $this->m_destInstance->selectDb();
      
      $result.= '<table border="0">';
      for ($i=0;$i<count($recordset);$i++)
      {
        $detaillink = "&nbsp;";
        $selector="";
        if (in_array($this->m_destInstance->primaryKey($recordset[$i]),$selectedPk))
        {
          $sel = "checked";
          if (!$this->m_linkInstance->hasFlag(NF_NO_EDIT)&&$this->m_linkInstance->allowed("edit"))
          {
            $selector = $this->localKey().
                                "='".$record[$this->m_ownerInstance->primaryKeyField()]."'".
                        ' AND '.$this->remoteKey().
                                "='".$recordset[$i][$this->m_destInstance->primaryKeyField()]."'";            
            
            // Create link to details.
            //var_dump($recordset[$this->fieldName()]);            
            $detaillink = href(dispatch_url($this->m_link, 
                                            "edit", 
                                            array("atkselector"=>$selector)),
                               "[".text("details")."]", 
                               SESSION_NESTED, 
                               true);
          }
        }
        else
        {
          $sel = "";          
        }
        $result.= '<tr><td class="table"><input type="checkbox" name="'.
                       $fieldprefix.$this->fieldName().
                       '_AMDAE_'.$this->remoteKey().
                       '[]" value="'.
                       $recordset[$i][$this->m_destInstance->primaryKeyField()].
                       '" '.$sel.'></td><td class="table">'.
                       $this->m_destInstance->descriptor($recordset[$i]).'</td><td class="table">'.$detaillink.'</td></tr>'."\n";
      }
      $result.='</table>';
      return $result;
    }

    /**
     * Stores the values in the database
     * @param $notused Not used
     * @param $record Current record
     */
    function store($notused, $record, $mode)
    {
      if ($this->createDestination())
      {
        $this->createLink();      
        $rel = &$this->m_linkInstance;
        
        // Find items that are checked.
        $checked = array();
        for ($i=0;$i<count($record[$this->fieldName()]);$i++)
        {
          $checked[] = $record[$this->fieldName()][$i][$this->remoteKey()][$this->m_destInstance->primaryKeyField()];
        }                
  
        $filter = $this->localKey()."=".$record[$this->m_ownerInstance->primaryKeyField()];
        if(count($checked)>0) $filter.=" AND ".$this->remoteKey()." NOT IN (".implode(",",$checked).")";
        $rel->deleteDb($filter);
        
        for ($i=0;$i<count($record[$this->fieldName()]);$i++)
        {
          // magical stuff..
          $newrecord = $record[$this->fieldName()][$i];
          //$newrecord[$this->remoteKey()][$this->m_destInstance->primaryKeyField()] = $record[$this->fieldName()][$i][$this->remoteKey()];
          $newrecord[$this->localKey()][$this->m_ownerInstance->primaryKeyField()] = $record[$this->m_ownerInstance->primaryKeyField()];
         
          // First check if the record does not exist yet. 
          /* @var $rel atkNode */
          $existing = $rel->selectDb($rel->primaryKey($newrecord), "", "", "", $rel->m_primaryKey);
          if (!count($existing))
          {  
            $rel->addDb($newrecord, true, $mode);
          }
        }
      }
    }
    
    function isEmpty($postvars)
    {
      return (!is_array($postvars[$this->fieldName()]) || count($postvars[$this->fieldName()])<=0);
    }
    
    function fetchValue($record)
    {
      $this->createDestination();      
      $result = array();
      for ($i=0;$i<count($record[$this->fieldName()]);$i++)
      {
        // magical stuff..
        $newrecord = array();
        $newrecord[$this->remoteKey()][$this->m_destInstance->primaryKeyField()] = $record[$this->fieldName()][$i][$this->remoteKey()];
        $result[] = $newrecord;
      }      
      return $result;
    }

  }
