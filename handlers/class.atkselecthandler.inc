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
   * @author Ivo Jansch <ivo@achievo.org>
   * @author Peter C. Verhage <peter@ibuildings.nl>
   * @package atk
   * @subpackage handlers
   *
   */
  class atkSelectHandler extends atkActionHandler
  {
    /**
     * The action handler method.
     */
    function action_select()
    {
      $output = $this->invoke("selectPage");
      
      if ($output != "")
      {
        if ($this->m_partial != NULL)
        {
          $output = $this->m_node->renderActionPage("select", $output);
        }
        
        $page =  &$this->getPage();
        $page->addContent($output);
      }
    }

    /**
     * This method returns an html page containing a recordlist to select
     * records from. The recordlist can be searched, sorted etc. like an
     * admin screen.
     *
     * @return String The html select page.
     */
    function selectPage()
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
                                   "select");

      if ($this->m_partial == NULL && count($recordset)==1 && $node->hasFlag(NF_AUTOSELECT))
      {
        // There's only one record and the autoselect flag is set, so we
        // automatically go to the target.
        atkimport("atk.utils.atkstringparser");
        $parser = new atkStringParser(rawurldecode(atkurldecode($node->m_postvars['atktarget'])));

        // for backwardscompatibility reasons, we also support the '[pk]' var.
        $recordset[0]['pk'] = $node->primaryKey($recordset[0]);
        $target = $parser->parse($recordset[0],true);

        $node->redirect(session_url($target, SESSION_NESTED));
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
        $container->setActions(array('select' => atkurldecode($node->m_postvars['atktarget'])));
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