<?php
/**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision$
   */

  /**
   * The atkController class 
   *
   * @author Maurice Maas <maurice@ibuildings.nl>
   * @package atk
   */
  class atkController
  {
    
    var $m_node = NULL;
    
    var $m_php_file = "";
    
    var $m_return_output = false;

    /**
     * List of actions that should give success/failure feedback
     * @access private
     * @var array
     */
    var $m_feedback = array();

    
    /**
     * Key/value Array containing which are be send als post or get vars
     * @access private
     * @var Array
     */   
    var $m_hidden_vars = array();

    
    function atkController(&$node)
    {
      $this->m_node = &$node;      
    }
        
    /**
     * This is the wrapper method for all http requests on a node.
     *
     * The method looks at the atkaction from the postvars and determines what
     * should be done. If possible, it instantiates actionHandlers for
     * handling the actual action.
     *
     * @param array $postvars The request variables for the node.
     * @param boolean $fullpage If true (default), the dispatch function
     *                          renders a complete html page, including
     *                          <html><head><body> etc. If you set this to
     *                          false, plain output is generated. Warning: If
     *                          you set this to false, you are responsible for
     *                          loading all the required scripts, stylesheets
     *                          etc. (When $fullpage is true, these are
     *                          automatically included in the page header.
     */
    function dispatch($postvars, $fullpage=true)
    {
      // Use invoke to be backwards compatible with overrides 
      // of loadDispatchPage in atknode.
      $this->invoke("loadDispatchPage", $postvars);
      //$this->loadDispatchPage($postvars);

      $ui = &$this->m_node->getUi();
      $page = &$this->m_node->getPage();
      $screen = $page->render(atktext('app_shorttitle')." - ".$ui->title($this->m_node->m_module, $this->m_node->m_type, $this->m_node->m_postvars['atkaction']), $fullpage);

      if(!$this->m_return_output)
      {
        $output = &atkOutput::getInstance();
        $output->output($screen);
      }

      // This is the end of all things for this page..
      // so we clean up some resources..
      $db = &$this->m_node->getDb();
      if (is_object($db)) $db->disconnect();
      atkdebug("disconnected from the database");
      
      if($this->m_return_output)
      {
        atkdebug("atkController::dispatch Return output ");
        return $screen;
      }
    }

    /**
     * Does the actual loading of the dispatch page
     * And adds it to the page for the dispatch() method to render.
     * @param array $postvars The request variables for the node.
     */
    function loadDispatchPage($postvars)
    {
      $this->m_node->m_postvars = $postvars;

      $this->m_node->m_action = $postvars['atkaction'];

      /** DIRTY HACK
          if action is something else than edit, we clear the atkformdata sessionstuff **/
      if ($this->m_node->m_action!="edit"&&$this->m_node->m_action!="add"&&$this->m_node->m_action!="admin")
      {
        atkdebug("Clearing stored formdata");
        $this->m_node->m_postvars["atkformdata"]="";
      }

      $page = &$this->m_node->getPage();

      if ($this->m_node->allowed($this->m_node->m_action))
      {
        $secMgr = &atkGetSecurityManager();
        $secMgr->logAction($this->m_node->m_type, $this->m_node->m_action);
        $this->m_node->callHandler($this->m_node->m_action);

        list($selector,$id) = explode("=", $this->m_node->m_postvars["atkselector"]);
        $page->register_hiddenvars(array("atknodetype"=>$this->m_node->m_module.".".$this->m_node->m_type, "atkselector"=>str_replace("'", "", $id)));
      }
      else
      {
        $page->addContent($this->genericPage(atktext('access_denied'), "<br><br>".atktext("error_node_action_access_denied", "", $this->m_node->m_type)."<br><br><br>"));
      }
    }

    /**
     * Render a generic page, with a box, title, stacktrace etc.
     * @param String $title The title of the page. Appears in the box title
     *                      and in the page header.
     * @param String $content The content to display in the box.
     * @return String A complete html page with the desired content.
     */
    function genericPage($title, $content)
    {
       $ui = &$this->m_node->getUi();
       $this->m_node->addStyle("style.css");
       return $this->invoke("renderActionPage", $title, $ui->renderBox(array("title"=>$title,
                                                                   "content"=>$content)));
    }

    /**
     * Render a generic action.
     *  
     * Renders actionpage.tpl for the desired action. This doesn't include a
     * box. The output can be rendered in a box with atkUi's renderBox method
     * if desired.
     * @param String $action The action for which the page is rendered.
     * @param mixed $blocks Pieces of html content to be rendered. Can be a
     *                      single string with content, or an array with
     *                      multiple content blocks.
     * @param String HTLM fragment for the action.
     * @param String $content The content to display in the box.
     * @return String A complete html page with the desired content.
     */
    function renderActionPage($action, $blocks=array())
    {
      if(!is_array($blocks))
      {
        $blocks = ($blocks==""?array():array($blocks));
      }

      $ui = &$this->m_node->getUi();

      // todo: overridable action templates
      return $ui->render("actionpage.tpl", array("blocks"=>$blocks,
                                                 "title"=>$ui->title($this->m_node->m_module, $this->m_node->m_type, $action)));
    }

    /**
     * Use this function to enable feedback for one or more actions.
     *
     * When feedback is enabled, the action does not immediately return to the
     * previous screen, but first displays a message to the user. (e.g. 'The
     * record has been saved').
     *
     * @param mixed $action The action for which feedback is enabled. You can
     *                      either pass one action or an array of actions.
     * @param int $statusmask The status(ses) for which feedback is enabled.
     *                        If for example this is set to ACTION_FAILED,
     *                        feedback is enabled only when the specified
     *                        action failed. It is possible to specify more
     *                        than one status by concatenating with '|'.
     */
    function setFeedback($action, $statusmask)
    {
      if (is_array($action))
      {
        for ($i=0, $_i=count($action); $i<$_i; $i++)
        {
          $this->m_feedback[$action[$i]] = $statusmask;
        }
      }
      else
      {
        $this->m_feedback[$action] = $statusmask;
      }
    }
    
    
    /**
     * Determine the url for the feedbackpage.
     *
     * Output is dependent on the feedback configuration. If feedback is not
     * enabled for the action, this method returns an empty string, so the
     * result of this method can be passed directly to the redirect() method
     * after completing the action.
     *
     * The $record parameter is ignored by the default implementation, but
     * derived classes may override this method to perform record-specific
     * feedback.
     * @param String $action The action that was performed
     * @param int $status The status of the action.
     * @param array $record The record on which the action was performed.
     * @param String $message An optional message to pass to the feedbackpage,
     *                        for example to explain the reason why an action
     *                        failed.
     * @return String The feedback url.
     */
    function feedbackUrl($action, $status, $record="", $message="")
    {
      if((isset($this->m_feedback[$action]) && hasFlag($this->m_feedback[$action], $status)) || $status==ACTION_FAILED)
        $vars = array("atkaction"=>"feedback", "atkfbaction"=>$action, "atkactionstatus"=>$status,"atkfbmessage"=>$message);
      atk_var_dump($vars, "atkController::feedbackUrl()");
      
      $dispatch_url = $this->dispatchUrl($vars);
      atkdebug("dispatch_url " . $dispatch_url);
      
      return (session_url($dispatch_url,SESSION_REPLACE));
    }
    
    
    
    /**
     * Generate a dispatch menu URL for use with nodes
     * and their specific actions.
     * @param string $node the (module.)node name
     * @param string $action the atk action the link will perform
     * @param string $params: A key/value array with extra options for the url
     * @param string $phpfile The php file to use for dispatching, defaults to dispatch.php
     * @return string url for the node with the action
     */
    function dispatchUrl($params=array(), $atknodetype="", $file="")
    {
      
      $vars = array_merge($params, $this->m_hidden_vars);
      atk_var_dump($vars, "atkController::dispatchUrl()");
      
      if($file!="") $phpfile = $file;
      elseif($this->m_php_file!="") $phpfile = $this->m_php_file;
      else $phpfile = $_SERVER["PHP_SELF"];
      
      $dispatch_url = dispatch_url(($atknodetype=="" ? $this->m_node->atkNodeType() : $atknodetype), $vars["atkaction"], $vars, $phpfile);
      atkdebug("dispatch_url " . $dispatch_url);
      
      return (session_url($dispatch_url,SESSION_REPLACE));
      
    }        

    /**
     * Returns the form buttons for a certain page.
     *
     * Can be overridden by derived classes to define custom buttons.
     * @param String $mode The action for which the buttons are retrieved.
     * @param array $record The record currently displayed/edited in the form.
     *                      This param can be used to define record specific
     *                      buttons.
     */
    function getFormButtons($mode, $record)
    {
      atkdebug("atkController::getFormButtons()");
      $result = array();

      // edit mode
      if ($mode == "edit")
      {
        // if atklevel is 0 or less, we are at the bottom of the session stack,
        // which means that 'saveandclose' doesn't close anyway, so we leave out
        // the 'saveandclose' button. Unless, a feedback screen is configured.
        if (atkLevel() >0 || hasFlag($this->m_feedback["update"], ACTION_SUCCESS))
        {
          $result[] = '<input type="submit" class="btn_saveandclose" name="atksaveandclose" value="'.atktext('saveandclose', "atk").'">';
        }
        $result[] = '<input type="submit" class="btn_save" name="atknoclose" value="'.atktext("save", "atk").'">';
        $result[] = '<input type="submit" class="btn_cancel" name="atkcancel" value="'.atktext("cancel", "atk").'">';
      }
      elseif ($mode == "add")
      {
        $result[] = '<input type="submit" class="btn_save" value="'.atktext("save", "atk").'">';
        // if action is admin, we don't show the cancelbutton or the add next button
        if ($this->m_node->hasFlag(NF_ADDAFTERADD) && !$this->m_node->hasFlag(NF_EDITAFTERADD))
          $result[] = '<input type="submit" class="btn_saveandnext" name="atksaveandnext" value="'.atktext("saveandnext", "atk").'">';
        $result[] = '<input type="submit" class="btn_cancel" name="atkcancel" value="'.atktext("cancel", "atk").'">';
      }
      elseif ($mode == "view")
      {
        if(atkLevel() > 0)
          $result[] = '<input type="submit" class="btn_cancel" name="atkback" value="&lt;&lt; '.atktext('back').'">';
        // if appropriate, display an edit button.
        if (!$this->m_node->hasFlag(NF_NO_EDIT)&&$this->m_node->allowed("edit", $record))
        {
          $result[] = '<input type="hidden" name="atkaction" value="edit">'.
                      '<input type="hidden" name="atknodetype" value="'.$this->m_node->atkNodeType().'">'.
                      '&nbsp;<input type="submit" class="btn_save" value="'.atktext('edit').'">&nbsp;';
        }
      }

      return $result;
    }

    /**
     * Set Key/value pair in m_hidden_vars array. Saved pairs are
     * send as post or get vars in the next page load
     *
     * @param string $name the reference key 
     * @param string $value the actual value
     */     
    function setHiddenVar($name, $value)
    {
      $this->m_hidden_vars[$name] = $value;
    }
    
    /**
     * Return m_hidden_vars array.
     *
     * @return array
     */
    function getHiddenVars()
    {
      return $this->m_hidden_vars;
    }

    /**
     * Return m_hidden_vars as html input types.
     *
     * @return string
     */
    function getHiddenVarsString()
    {
      atk_var_dump($this->m_hidden_vars, "m_hidden_vars");
      if (count($this->m_hidden_vars) == 0) return "";
      foreach($this->m_hidden_vars as $hiddenVarName => $hiddenVarValue)
      {
        $varString .= '<input type="hidden" name="'.$hiddenVarName.'" value="'.$hiddenVarValue.'">';
      }
      return $varString;
    }
    
    function setReturnOutput($returnOutput)
    {
      $this->m_return_output = $returnOutput;
    }
    
    /**
     * Generic method invoker (copied from class.atkactionhandler.inc).
     *
     * Controller methods invoked with invoke() instead of directly, have a major
     * advantage: the controller automatically searches for an override in the
     * node. For example, If a controller calls its getSomething() method using
     * the invoke method, the node may implement its own version of
     * getSomething() and that method will then be called instead of the
     * original. The controller is passed by reference to the override function
     * as first parameter, so if necessary, you can call the original method
     * from inside the override.
     *
     * The function accepts a variable number of parameters. Any parameter
     * that you would pass to the method, can be passed to invoke(), and
     * invoke() will pass the parameters on to the method. 
     *
     * There is one limitation: you can't pass parameters by reference if
     * you use invoke().
     *
     * <b>Example:</b>
     *
     * <code>
     *   $controller->invoke("dispatch", $postvars, $fullpage);
     * </code>
     *
     * This will call dispatch(&$handler, $postvars, $fullpage) on your node class
     * if present, or dispatch($record, $postvars, $fullpage) in the handler if the node has
     * no override.
     *
     * @param String $methodname The name of the method to call.
     * @return mixed The method returns the return value of the invoked
     *               method.
     */
    function invoke($methodname)
    {            
      $arguments = func_get_args(); // Put arguments in a variable (php won't let us pass func_get_args() to other functions directly.      
      // the first argument is $methodname, which we already defined by name.      
      array_shift($arguments);
      
      if ($this->m_node!==NULL && method_exists($this->m_node, $methodname)) 
      {
        atkdebug("Invoking '$methodname' override on node");
        // We pass the original object as last parameter to the override. 
        array_push($arguments, $this);
        return call_user_func_array(array(&$this->m_node, $methodname), $arguments);
      }
      else if (method_exists($this, $methodname)) 
      {
        atkdebug("Invoking '$methodname' on controller");
        return call_user_func_array(array(&$this, $methodname), $arguments);
      }
      atkerror("Undefined method '$methodname' in atkController");
    }
    
  }
?>