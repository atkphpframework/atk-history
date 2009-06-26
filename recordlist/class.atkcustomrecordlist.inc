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
  	protected $m_mode;
  	protected $m_attribList = array();
    protected $sol;
    protected $sof;
    protected $eof;
    protected $eol;
    protected $fsep;
    protected $empty;

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
     * @param String $fsep        String to use between fields
     * @param String $lfreplace   String for replacing line feeds in recordset field values (null = do not replace)
    */
    function render(&$node, $recordset, $sol, $sof, $eof, $eol, $type="0", $compression="", $suppressList="",
                    $outputparams=array(), $mode="list", $titlerow=true, $decode=false, $fsep="", $rfeplace=null)
    {
      $this->setNode($node);
      $this->m_mode = $mode;

      // example      html         csv

      // $sol     = '<tr>'         or  ''
      // $sof     = '<td>'         or  '"'
      // $eof     = '</td>'        or  '"'
      // $eol     = '</tr>'        or  '\r\n'
      // $fsep    = ''             or  ';'
      //$empty  om lege tabelvelden op te vullen;

      // stuff for the totals row..
      $totalisable = false;
      $totals = array();
      if ($type=="0")
      {
        $this->empty = "&nbsp;";
      }
      if ($type=="1")
      {
        $output = "";
        $this->empty = "";
      }
      $this->sol = $sol;
      $this->sof = $sof;
      $this->eof = $eof;
      $this->eol = $eol;
      $this->fsep = $fsep;

      $funcList = array();

      foreach (array_keys($this->m_node->m_attribList) as $attribname)
      {
        $p_attrib = &$this->m_node->m_attribList[$attribname];
        $musthide=(is_array($suppressList)&&count($suppressList)>0&&in_array($attribname,$suppressList));
        if (!$this->isHidden($p_attrib) && !$musthide)
        {
          $this->m_attribList[] = &$p_attrib;

          $funcname = $attribname."_display";
          if (method_exists($this->m_node,$funcname))
          {
            $funcList[] =array(&$this->m_node,$funcname);
          }
          else
          {
            // otherwise, the display function of the particular attribute
            // is called.
            $funcList[] = array(&$p_attrib,"display");
          }

          // the totalisable check..
          // Since we are looping the attriblist anyway, we also check if there
          // are totalisable collumns.
          if ($p_attrib->hasFlag(AF_TOTAL))
          {
            $totalisable = true;
          }
        }
      }

      if ($titlerow)
      {
        $output .= $this->sol;

        // display a headerrow with titles.
        for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
        {
          $output.=$this->sof.$this->eolreplace($this->m_attribList[$j]->label(), $rfeplace).$this->eof.$this->fsep;
        }
        $output.=$this->eol;
      }

      // Display the values
      $columnConfig = &$this->m_node->getColumnConfig();

      if($columnConfig->hasSubTotals())
      {
        $result = array();
        $lastvalues = array();

        $totalizers = $columnConfig->totalizableColumns();
        $subtotalfields = $columnConfig->subtotalColumns();

        $subtotals = array();

        for ($i = 0, $_i = count($recordset); $i < $_i; $i++)
        {
          for ($j=count($subtotalfields)-1; $j>=0; $j--) // reverse loop, to ensure right-to-left subtotalling
          {
            $fieldname = $subtotalfields[$j];
            $value = $recordset[$i][$fieldname];
            $p_subtotalling_attrib = &$this->m_node->getAttribute($fieldname);

            if (isset($lastvalues[$fieldname]) && !$p_subtotalling_attrib->equal($recordset[$i], $lastvalues))
            {
              $output .= $this->_subTotalRow($recordset[$i], $subtotals, $fieldname, $totalizers);
            }

            foreach($totalizers as $totalfield)
            {
              $p_attrib = &$this->m_node->getAttribute($totalfield);
              $subtotals[$totalfield][$fieldname] = $p_attrib->sum($subtotals[$totalfield][$fieldname], $recordset[$i]);
            }
            $lastvalues[$fieldname] = $value;
          }

          $output.=$this->sol;
          for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
          {
            $value = $funcList[$j][0]->$funcList[$j][1]($recordset[$i], $this->m_mode);
            $output.=$this->sof.($value==""?$this->empty:$value).$this->eof.$this->fsep;

            // Calculate totals..
            if ($this->m_attribList[$j]->hasFlag(AF_TOTAL))
            {
              $totals[$this->m_attribList[$j]->fieldName()] = $this->m_attribList[$j]->sum($totals[$this->m_attribList[$j]->fieldName()], $recordset[$i]);
            }
          }
          $output .= $this->eol;
        }
        // leftovers, subtotals of last rows
        if (count($recordset))
        {
          for ($j=count($subtotalfields)-1; $j>=0; $j--) // reverse loop, to ensure right-to-left subtotalling
          {
            $fieldname = $subtotalfields[$j];
            $output .= $this->_subTotalRow($recordset[count($recordset)-1], $subtotals, $fieldname, $totalizers);
          }
        }
        // end of leftovers
      }
      elseif ($totalisable)
      {
        for ($i = 0, $_i = count($recordset); $i < $_i; $i++)
        {
          $output.=$this->sol;
          for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
          {
            $value = $funcList[$j][0]->$funcList[$j][1]($recordset[$i], $this->m_mode);
            $output.=$this->sof.($value==""?$this->empty:$value).$this->eof.$this->fsep;

            // Calculate totals..
            if ($this->m_attribList[$j]->hasFlag(AF_TOTAL))
            {
              $totals[$this->m_attribList[$j]->fieldName()] = $this->m_attribList[$j]->sum($totals[$this->m_attribList[$j]->fieldName()], $recordset[$i]);
            }
          }
          $output.=$this->eol;
        }
      }
      else
      {
        for ($i = 0, $_i = count($recordset); $i < $_i; $i++)
        {
          $output.=$this->sol;
          for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
          {
            $value = $funcList[$j][0]->$funcList[$j][1]($recordset[$i], $this->m_mode);
            $output.=$this->sof.($value==""?$this->empty:$value).$this->eof.$this->fsep;
          }
          $output.=$this->eol;
        }
      }

      // totalrow..
      if ($totalisable)
      {
        $totalRow = $this->sol;

        // Third loop.. this time for the totals row.
        for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
        {
          if ($this->m_attribList[$j]->hasFlag(AF_TOTAL))
          {
            $value = $this->eolreplace($this->m_attribList[$j]->display($totals[$this->m_attribList[$j]->fieldName()], $this->m_mode), $rfeplace);
            $totalRow.=$this->sof.($value=="" ? $this->empty : $value).$this->eof.$this->fsep;
          }
          else
          {
            $totalRow.= $this->sof.$this->empty.$this->eof.$this->fsep;
          }
        }

        $totalRow .= $this->eol;

        $output .= $totalRow;

      }

      // html requires table tags
      if ($type == "0")
      {
        $output = '<table border="1" cellspacing="0" cellpadding="2">'.$output."</table>";
      }

      // To a File
      if (!array_key_exists("filename", $outputparams))
        $outputparams["filename"] = "achievo";

      if ($this->m_exportcsv)
      {
        $ext = ($type == "0" ? "html" : "csv");
        $exporter = &atknew("atk.utils.atkfileexport");
        $exporter->export($output, $outputparams["filename"], $ext, $ext, $compression);
      }
      else
      {
      	return $output;
      }
    }

    protected function isHidden(atkAttribute $attribute)
    {
      if ($attribute->hasFlag(AF_HIDE)) return true;
      if ($attribute->hasFlag(AF_HIDE_SELECT) && $this->m_node->m_action==='select') return true;
      if ($attribute->hasFlag(AF_HIDE_LIST) && ($this->m_node->m_action==='export' || $this->m_mode==='export')) return true;
      return false;
    }

    function setExportingCSVToFile($export=true)
    {
    	if (is_bool($export)) $this->m_exportcsv = $export;
    }

    /**
     * Replace any eol character(s) by something else
     *
     * @param String $string        The string to process
     * @param String $replacement   The replacement string for '\r\n', '\n' and/or '\r'
     */
    function eolreplace($string, $replacement)
    {
      if (!is_null($replacement))
      {
        $string = str_replace("\r\n", $replacement, $string); // prevent double replacement in the next lines!
        $string = str_replace("\n", $replacement, $string);
        $string = str_replace("\r", $replacement, $string);
      }

      return $string;
    }

    private function _subTotalRow($row, &$totals, $fieldforsubtotal, $totalizers)
    {
      $subtotalcols = array();
      foreach($totalizers as $totalfield)
      {
        $p_attrib = &$this->m_node->getAttribute($totalfield);
        $subtotalcols[$totalfield] = $p_attrib->display($totals[$totalfield][$fieldforsubtotal]);

        // reset walking total
        $totals[$totalfield][$fieldforsubtotal] = "";
      }


      return $this->_createSubTotalRowFromRow($row, $fieldforsubtotal, $subtotalcols);
    }

    function _createSubTotalRowFromRow($row, $fieldname, $subtotalcolumns)
    {
      $subRow =  $this->sol;
      for ($j=0, $_j=count($this->m_attribList); $j<$_j; $j++)
      {
        $col = $this->m_attribList[$j]->fieldname();
        if ($col==$fieldname)
        {
          $value = atktext("subtotal");
        }
        elseif (isset($subtotalcolumns[$col]))
        {
          $value = $subtotalcolumns[$col];
        }
        else
        {
          $value = $this->empty;
        }
        $subRow .= $this->sof.$value.$this->eof.$this->fsep;
      }
      return $subRow.$this->eol;
    }

  }

?>
