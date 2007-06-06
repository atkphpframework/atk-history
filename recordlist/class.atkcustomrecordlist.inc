<?php

  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be 
   * included in the distribution.
   *
   * @package atk
   * @subpackage recordlist
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /** @internal include base class */
  atkimport("atk.recordlist.atkrecordlist");

  /**
   * Custom recordlist renderer.
   *
   * @author Paul Verhoef <paul@ibuildings.nl>
   * @package atk
   * @subpackage recordlist
   *
   */
  class atkCustomRecordList extends atkRecordList 
  {    
  	var $m_exportcsv = true;
  	
    /**
     * Creates a special Recordlist that can be used for exporting to files or to make it printable
     * @param atkNode $node       The node to use as definition for the columns.
     * @param array $recordset    The records to render
     * @param String $sol         String to use at start of each row     
     * @param String $sof         String to use at start of each field
     * @param String $eof         String to use at end of each field     
     * @param String $eol         String to use at end of each row
     * @param int $type           0=Render rows in simple html tabl; 1= raw export
     * @param $compression        Compression technique (bzip / gzip)
     * @param array $suppressList List of attributes from $node that should be ignored  
     * @param array $outputparams Key-Value parameters for output. Currently existing: 
     *                               filename - the name of the file (without extension .csv)     
     * @param String $mode	      The mode that is passed to attributes' display() method 
     *                            (for overrides). Defaults to 'list'.
     * @param Boolean $titlerow   Should titlerow be rendered or not
     * @param Boolean $decode     Should data be decoded or not (for exports)
    */
    function render(&$node, $recordset, $sol, $sof, $eof, $eol, $type="0", $compression="", $suppressList=array(), $outputparams=array(), $mode="list", $titlerow=true, $decode=false)
    {
      $this->setNode($node);
      if(!is_array($suppressList)) $suppressList = array();

      // stuff for the totals row..
      if ($type=="0")
      {
        $empty = "&nbsp;";
      }
      if ($type=="1")
      {
        $empty="";
      }

      // stuff for the totals row..
      $totalrow = array();
      $ar = array();
      $p_attribList = array();

      foreach (array_keys($this->m_node->m_attribList) as $attribname)
      {
        $p_attrib = &$this->m_node->getAttribute($attribname);
        if (!$p_attrib->hasFlag(AF_HIDE_LIST) && !in_array($attribname,$suppressList))          
        {
          $p_attribList[] = &$p_attrib;
        }
      }

      if ($titlerow)
      {
        $row =array();

        // display a headerrow with titles.
        for ($j=0, $_j=count($p_attribList); $j<$_j; $j++)
        {
          $p_attribList[$j]->addToExportArrayHeader($row, $suppressList);
        }
        $ar[] = $row;
      }
             
      for ($i = 0, $_i = count($recordset); $i < $_i; $i++)
      {
        $row = array();
        for ($j=0, $_j=count($p_attribList); $j<$_j; $j++)
        {
          $p_attribList[$j]->addToExportArrayRow($recordset[$i],$row, $totalrow, $suppressList, $mode, $decode, $empty);
        }

        $ar[] = $row;
      }

      // totalrow..
      if (count($totalrow)<>0)
      {
        $row = array();

        // Third loop.. this time for the totals row.
        for ($j=0, $_j=count($p_attribList); $j<$_j; $j++)
        {
          $p_attribList[$j]->addToExportArrayTotals($row,  $totalrow, $suppressList, $mode, $empty);
        }
        
        $ar[] = $row;
      }
      
      atkdebug("Start render");

      $tbl = &atknew('atk.utils.atkcustomtablerenderer');
      return $tbl->render($ar, $sol, $sof, $eof, $eol, $type, $compression, $suppressList,$outputparams, $mode, $titlerow, $decode, $this->m_exportcsv);
    }
        
    function setExportingCSVToFile($export = false)
    {
    	if (is_bool($export)) $this->m_exportcsv = $export;
    }
  }

?>