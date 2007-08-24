<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage relations
   *
   * @copyright (c)2006 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

   /** @internal base include */
   userelation("atkmanytomanyrelation");

  /**
    * Many-to-many relation.
    *
    * The relation shows two lists, one with elements that are currently
    * linked to the master record, and one with available records to choose from
    *
    * @author Ivo Jansch <ivo@achievo.org>
    * @package atk
    * @subpackage relations
    *
    */
  class atkShuttleRelation extends atkManyToManyRelation
  {
    /**
     * Return a piece of html code to edit the attribute
     * @param $record Current record
     * @return String piece of html code
     */
    function edit($record, $fieldprefix, $mode)
    {
      $this->createDestination();
      $this->createLink();

      $recordset = $this->_getSelectableRecords($record, $mode);
      $selected = $this->getSelectedRecords($record);

      $shuttledata = $this->getShuttleData($recordset,$selected);

      return $this->getHTML($fieldprefix, $shuttledata);
    }

    function getShuttleData($recordset,$selected)
    {
      $left = array();
      $right = array();
      $width=100;

      for ($i=0;$i<count($recordset);$i++)
      {
        $recordset[$i]['__descriptor'] = $this->m_destInstance->descriptor($recordset[$i]);
        if (in_array($this->m_destInstance->primaryKey($recordset[$i]),$selected))
        {
          $right[$recordset[$i][$this->m_destInstance->primaryKeyField()]] = $recordset[$i]['__descriptor'];
        }
        else
        {
          $left[$recordset[$i][$this->m_destInstance->primaryKeyField()]] = $recordset[$i]['__descriptor'];
        }

        // fancy autowidth detection
        $width = max(strlen($recordset[$i]['__descriptor'])*10, $width);
      }
      return array('left'=>$left,'right'=>$right,'width'=>$width);
    }

    function getSelectedRecords($record)
    {
      $selectedPk = array();

      for ($i=0;$i<count($record[$this->m_name]);$i++)
      {
        if(is_array($record[$this->fieldName()][$i][$this->getRemoteKey()]))
          $newselected = $this->m_destInstance->primaryKey($record[$this->m_name][$i][$this->getRemoteKey()]);
        else
          $newselected = $this->m_destInstance->primaryKey(array($this->m_destInstance->primaryKeyField()=>
                                                                 $record[$this->m_name][$i][$this->getRemoteKey()]));
        $selectedPk[] = $newselected;
      }
      return $selectedPk;
    }

    function getHTML($fieldprefix, $data)
    {
      $leftname =  $fieldprefix.$this->fieldName()."_sel";
      $rightname = $fieldprefix.$this->fieldName().'[]['.$this->getRemoteKey().']';

      $this->registerJavascript($data,$leftname,$rightname,$fieldprefix);

      $result = "\n".'<table border="0"><tr><td>'.atktext('available', 'atk').':<br/>';
      $result.= $this->_renderSelect($leftname, $data['width']);
      $result.= "\n".'</td><td valign="center" align="center">';
      $result.=$this->getButtons();
      $result.= "\n".'</td><td>'.atktext('selected', 'atk').':<br/>';
      $result.= $this->_renderSelect($rightname, $data['width']);
      $result.= "\n".'</table>';

      return $result;
    }

    function registerJavascript($data,$leftname, $rightname,$fieldprefix="")
    {
      $json = &atknew('atk.utils.atkjson');
      $page = &$this->m_ownerInstance->getPage();
      $page->register_script(atkconfig("atkroot")."atk/javascript/class.atkshuttlerelation.js");
      $page->register_loadscript(  'ATK.shuttlerelation_'.$fieldprefix.$this->fieldName().
                                   " = new ATK.ShuttleRelation('".$fieldprefix.$this->fieldName()."',".
                                                              "'$leftname',". $json->encode($data['left']).
                                                             ",'$rightname',".$json->encode($data['right']).")");

      // on submit, we must select all items in the right selector, as unselected items
      // will not be posted.
      $page->register_submitscript('ATK.shuttlerelation_'.$fieldprefix.$this->fieldName().".selectAll('$rightname');");
    }

    function getButtons($fieldprefix="")
    {
      $prefix = $fieldprefix.$this->fieldName().'_';
      $result = "\n<input id='{$prefix}leftToRight'    type='button' value='&gt;' />    <br/>";
      $result.= "\n<input id='{$prefix}rightToLeft'    type='button' value='&lt;' />    <br/><br/>";
      $result.= "\n<input id='{$prefix}allLeftToRight' type='button' value='&gt;&gt;' /><br/>";
      $result.= "\n<input id='{$prefix}allRightToLeft' type='button' value='&lt;&lt;' />";
      return $result;
    }

    /**
     * Render the multiselect list control
     * @access private
     * @param String $name The name of the list control
     * @param array $recordset The list of records to render in the control
     * @param int $width The width of the control in pixels
     * @param String $opposite The name of the list control connected to this list control for shuttle actions
     * @return String piece of html code
     */
    function _renderSelect($id, $width)
    {
      $searchid = $id."_searchfield";
      $result= "<input id='$searchid' name='$searchid' type='text' style='width: {$width}px;' /><br />";

      $result.= "<select class='shuttle_select' id='$id' name='$id' size='10' style='width: {$width}px;' multiple='multiple'>";
      $result.= '</select>';
      return $result;
    }

    function isEmpty($postvars)
    {
      return (parent::isEmpty($postvars) && atk_value_in_array($postvars));
    }

    function store() {return true;}
  }

?>
