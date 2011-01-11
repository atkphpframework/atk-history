<?php
/**
 * This file is part of the Achievo ATK distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package atk
 * @subpackage utils
 *
 * @copyright (c) 2000-2011 Ibuildings.nl BV
 * 
 * @license http://www.achievo.org/atk/licensing ATK Open Source License
 */

/**
 * This component will enable the selection of items spanning multiple datagrid pages
 *
 * @package atk
 * @subpackage datagrid
 */
class atkDGMultiPageSelect extends atkDGList
{

  /**
   * Renders the component.
   * 
   * @return string component HTML
   */
  public function render()
  {
    $selection = $this->getGrid()->getPostvar('admin_atkselector');

    if (!(count($selection)>0)) return;
    $selection = '(' . implode(') OR (', $selection) . ')';


    $node = $this->getNode();
    $records = $node->selectDb($selection, "", "", "","", "", false, true);

    $grid    = $this->getGrid();
    $listname =  $grid->getName();
    
    $DGlist = $this->listArray($records, "", array(), array(), '');
    
    $recordListData = array(
        "listid" => $listname,
        "header" => $this->getRecordlistHeader($DGlist, $grid, $listname),
        "rows" =>   $this->getRecordlistRows($DGlist, $grid, $listname)
    );
    $ui = $grid->getNode()->getUi();

    return "<h3>".atktext("selected")."</h3>".$ui->render("recordlist.tpl", $recordListData);
  }

  /**
   * prepares some information for the recordlist template on which headers need to be shown.
   * 
   * @param array       $DGlist    
   * @param atkdatagrid $grid      current atkdatagrid instance
   * @param string      $listname  name for the list
   * @return array
   */
  private function getRecordlistHeader($DGlist, $grid, $listname)
  {
    $theme        = $this->getTheme();
    $orientation  = atkconfig('recordlist_orientation', $theme->getAttribute("recordlist_orientation"));

    $headercols   = array();
    $headercols[] = array("content" => "&nbsp;");

    if ($this->_hasActionColumn($DGlist) && count($DGlist["rows"]) == 0)
    {
      if ($orientation == "left" || $orientation == "both")
      {
        // empty cell above search button, if zero rows
        // if $orientation is empty, no search button is shown, so no empty cell is needed
        $headercols[] = array("content"=>"&nbsp;");
      }
    }

    if ($grid->hasFlag(atkDataGrid::LOCKING))
    {
      $lockHeadIcon = atkTheme::getInstance()->iconPath('lock_'.$grid->getNode()->getLockMode().'_head', 'lock', $grid->getNode()->m_module);
      $headercols[] = array("content"=>'<img src="'.$lockHeadIcon.'">');
    }
    if (($orientation == "left" || $orientation == "both") && ($this->_hasActionColumn($DGlist) && count($DGlist["rows"]) > 0))
    {
      $headercols[] = array("content"=>"");
    }

    foreach (array_values($DGlist["heading"]) as $head)
      $headercols[] = array("content"=>$head["title"]);

     return $headercols;
  }

  /**
   * Prepares the data to be shown in the recordlist template.
   *
   * @param  array       $records
   * @param  atkdatagrid $grid
   * @param  string      $listName
   * @return array
   */
  private function getRecordlistRows($records, $grid, $listName)
  {
    $theme = $this->getTheme();

    $defaulthighlight = $theme->getAttribute("highlight");

    $DGrecords = array();
    $idx = 1000000; // arbitrary large number to avoid clashing with the main recordlist.

    foreach ($records['rows'] as $record)
    {
      $idx++;
      $DGrecord = array();

      // Set the column for selecting the records.
      $DGrecord["cols"][] = array(
        "content" => '<input type="checkbox" name="'.$listName.'_atkselector['.$idx.']" value="'.$record["selector"].'"  onclick="if (this.disabled) this.checked = false" class="atkcheckbox" checked="checked" >'.
                    '<script language="javascript"  type="text/javascript">'.
                      $listName.'["'.atk_htmlentities($record["selector"]).'"] =
                      new Array("'.implode($record["mra"], '","').'");
                    </script>',
        "type" => "mra"
      );

      list($bgn, $bgh) = $this->getRecordBGColour($record, $grid->getNode(), $defaulthighlight);
      $DGrecord["background"] = $bgn;
      $DGrecord["highlight"]  = $bgh;
      $DGrecord["rownum"]     = $idx;
      $DGrecord["id"]         = $listName.'_'.$idx;
      $DGrecord["type"]       = $record["type"];

      /* locked? */
      if ($grid->hasFlag(atkDataGrid::LOCKING))
      {
        if (is_array($record["lock"]))
        {
          $this->getPage()->register_script(atkconfig('atkroot').'atk/javascript/overlibmws/overlibmws.js');
          $lockIcon = atkTheme::getInstance()->iconPath('lock_'.$grid->getNode()->getLockMode(), 'lock', $grid->getNode()->m_module);
          $lockInfo = addslashes(str_replace(array("\r\n", "\r", "\n"), " ", atk_htmlentities($this->getLockInfo($record["lock"]))));
          $DGrecord["cols"][] = array("content"=>'<img src="'.$lockIcon.'" onmouseover="return overlib(\''.$lockInfo.'\', NOFOLLOW, FULLHTML);" onmouseout="nd();" border="0">', "type" => "lock");
        }
        else $DGrecord["cols"][] = array("content"=>"");
      }

      foreach ($record["data"] as $html)
        $DGrecord["cols"][] = array("content"=>$html, "type" => "data");

      $DGrecords[] = $DGrecord;
    }

    return $DGrecords;
  }

  /**
   * Returns the colours for a row.
   *
   * Special rowColor method makes it possible to change the row color based on the record data.
   * the method can return a simple value (which will be used for the normal row color), or can be
   * an array, in which case the first element will be the normal row color, and the second the mouseover
   * row color, example: function rowColor(&$record, $num) { return array('red', 'blue'); }
   *
   * @param array   $record  atkRecord
   * @param atkNode $node
   * @param string  $defaulthighlight
   * @param string  $defaultbackground
   * @return array  the background colour and background highlight colour
   */
  private function getRecordBGColour($record, $node, $defaulthighlight='', $defaultbackground='')
  {
      $method = "rowColor";
      $bgn = $defaultbackground;
      $bgh = $defaulthighlight;
      if (method_exists($node, $method))
      {
        $bgn = $node->$method($record);
        if (is_array($bgn)) list($bgn, $bgh) = $bgn;
      }

      return array($bgn, $bgh);
  }
}