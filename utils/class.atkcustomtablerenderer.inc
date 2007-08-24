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
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Table renderer class. Makes rendering of html tables easier.
   *
   * The current implementation cannot handle merged cells (rowspan/colspan).
   * If you need this, you should render the table manually.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage utils
   *
   */
  class atkCustomTableRenderer
  {
    /**
     * Default constructor
     */
    function atkCustomTableRenderer()
    {
    }

    /**
     * Render a table.
     *
     * <b>Example:</b>
     * <code>
     *  $tbl = &atknew("atk.utils.atktablerenderer");
     *  $tbl->render($data, TBL_HEADER|TBL_ALTERNATE, "recordlist");
     * </code>
     *
     * @param array  $data   A multidimensional array containing the rows and
     *                       columns with data. The first dimension represents
     *                       the rows. The second dimension represents the
     *                       cols.
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
     * @return String The rendered table.
     */
    function render($data, $sol, $sof, $eof, $eol, $type="0", $compression="", $suppressList="",
                    $outputparams=array(), $mode="list", $titlerow=true, $decode=false, $exportcsv = true)
    {
      $output="";
      // example      html         csv
      // $sol     = "<tr>"         or  ""
      // $eof     = "</td>"        or  ";"
      // $sof     = "<td>"         or  ""
      // $eol     = "</tr>"        or  "\r\n"
      //$empty  om lege tabelvelden op te vullen;

      if ($type=="0")
      {
        $output = '<table border="1" cellspacing="0" cellpadding="4">';
      }
      if ($type=="1")
      {
        $output="";
      }

      
      for ($row=0, $_row=count($data); $row<$_row; $row++)
      {
        $output .= $sol.$sof.implode($eof.$sof, $data[$row]).$eof.$eol;
      }
      
      // To the Screen
      if ($type=="0")
      {
        
        $output .= "</table>";
        return $output;
      }
      // To a File
      if ($type=="1") 
      {
        if (!array_key_exists("filename", $outputparams))
          $outputparams["filename"] = "achievo";

        if ($exportcsv)
        {
          $exporter = &atknew("atk.utils.atkfileexport");        
          $exporter->export($output, $outputparams["filename"],"csv","csv",$compression);        
        }
        else 
        {
        	return $output;
        }
      }
    }
      
  }
?>