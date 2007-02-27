<?php
 /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage handlers
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   * $Id$
   */

  /**
   * Handler class for the select action of a node. The handler draws a
   * generic select form for searching through the records and selecting
   * one of the records.
   *
   * @author Lineke Kerckhoffs-Willems <lineke@ibuildings.nl>
   * @package atk
   * @subpackage handlers
   *
   */
  class atkMultiSelectHandler extends atkActionHandler
  {
    /**
     * The action handler method.
     */
    function action_multiselect()
    {
      if (isset($this->m_postvars['atkselector']))
      {
        $output = $this->invoke("handleMultiselect");
      }
      else $output = $this->invoke("multiSelectPage");
      
      if ($output != "")
      {
        if ($this->m_partial != NULL)
        {
          $output = $this->m_node->renderActionPage("multiselect", $output);
        }
        
        $page =  &$this->getPage();
        $page->addContent($output);
      }
    }
    
    /**
     * Parse atkselectors in postvars into atktarget using atktargetvartpl and atktargetvar
     * Then redirect to atktarget
     *
     */
    function handleMultiselect()
    {
      $node = &$this->m_node;
      $columnConfig = &$node->getColumnConfig();
      $recordset = $node->selectDb(implode(' OR ',$this->m_postvars['atkselector']),
                                   $columnConfig->getOrderByStatement(),
                                   "",
                                   $node->m_listExcludes,
                                   "",
                                   "multiselect");
                                   
      // loop recordset to parse atktargetvar
      $atktarget    = atkurldecode($node->m_postvars['atktarget']);
      $atktargetvar = $node->m_postvars['atktargetvar'];
      $atktargettpl = $node->m_postvars['atktargetvartpl'];
      
      for ($i = 0; $i < count($recordset); $i++) 
      {
      	if ($i == 0 && strpos($atktarget, '&') === false) $atktarget.= '?';
      	else $atktarget .= '&';
      	$atktarget.= $atktargetvar.'[]='.$this->parseString($atktargettpl, $recordset[$i]);
      }
      $node->redirect($atktarget);
    }
    
    function parseString($string, $recordset)
    {
      atkimport("atk.utils.atkstringparser");
      $parser = new atkStringParser($string);
      
      // for backwardscompatibility reasons, we also support the '[pk]' var.
      $recordset['pk'] = $this->m_node->primaryKey($recordset);
      $output = $parser->parse($recordset,true);
      return $output;
    }

    /**
     * This method returns an html page containing a recordlist to select
     * records from. The recordlist can be searched, sorted etc. like an
     * admin screen.
     *
     * @return String The html select page.
     */
    function multiSelectPage()
    {
      $node = &$this->m_node;
      $node->addStyle("style.css");
      // When there's a lot of data, records will be spread across multiple pages.
      if ($node->m_postvars['atklimit']=="") $node->m_postvars['atklimit']=atkconfig("recordsperpage");
      if ($node->m_postvars['atkstartat']=="") $node->m_postvars['atkstartat']=0;

      $filter = $node->validateFilter($node->m_postvars["atkfilter"]);

      $columnConfig = &$node->getColumnConfig();

      $recordset = $node->selectDb($filter,
                                   $columnConfig->getOrderByStatement(),
                                   array("offset" => $node->m_postvars['atkstartat'], "limit" => $node->m_postvars['atklimit']),
                                   $node->m_listExcludes,
                                   "",
                                   "multiselect");

      if ($this->m_partial == NULL && count($recordset)==1 && $node->hasFlag(NF_AUTOSELECT))
      {
        if ((atkLevel() > 0) && (atkArrayNvl($node->m_postvars, "atkprevlevel", 0) > atkLevel()))
        {
          $backlinkurl = session_url($_SERVER["PHP_SELF"].'?atklevel='.session_level(SESSION_BACK));
          $node->redirect($backlinkurl);
        }
        else
        {
          // There's only one record and the autoselect flag is set, so we
          // automatically go to the target.
          $target = $this->parseString(rawurldecode(atkurldecode($node->m_postvars['atktarget'])), $recordset[0]);
          $node->redirect(session_url($target, SESSION_NESTED));
        }
        return "";
      }
      else
      {
        $ui = &$this->getUi();
        
        /* @var $container atkRecordListContainer */
        $container = &atknew("atk.recordlist.atkrecordlistcontainer");
        $container->setNode($this->m_node);
        $container->setAction($this->m_node->m_action);
        $container->setRecords($recordset);
        $container->setActions(array('mra'=>array('multiselect'), 'actions'=>array()));
        if (!empty($this->m_postvars['atkrecordlist']))
          $container->setName($this->m_postvars['atkrecordlist']);
        $output = $container->render($this->m_partial != NULL);

        if ($this->m_partial == NULL)
          $output = $ui->renderBox(array("title"=>$node->actionTitle('select'), "content"=>$output));        
        
        return $output;
      }
    }
  }
?>