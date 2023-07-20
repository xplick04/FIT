<?php
/* @project IPP projekt 1 - parse.php
 * @author Maxim Plička, xplick04
 */
    ini_set('display_errors', 'stderr');

    //errors
    $argumentFormatError = 10;
    $outputError = 12;
    $headerError = 21;
    $opCodeError = 22;
    $otherOpCodeError = 23;

/**
 * @brief Function checks if arguments are in valid format
 * @return 0 if arguments are valid
 * @return 1 if --help or -help
 * @return 10 if arguments are in wrong format
 */
    function argumentCheck()
    {
        global $argc, $argv, $argumentFormatError;
        if($argc > 1)
        {
            if($argv[1] == "--help" && $argc == 2)
            {
                echo "Program se spouští ve formátu php parse.php <argumenty>, vstup se bere z STDIN a výstup je na STDOUT";
                return(1);
            }
            elseif(!strncmp($argv[1],"--stats=", 8)) //firstly file to output statisticks must be set
            {
                return(2);
            }
            return($argumentFormatError);
        }
        return(0);
    }

/**
 * @brief Function checks if string is variable
 * @param $string, potentional variable
 * @return true if $string is variable
 * @return false if $string isnt variable
*/
    function isVar($string)
    {
        if(preg_match("/^(LF|GF|TF)@[a-zA-Z_\-$&%*!?][a-zA-Z_\-$&%*!?0-9]*$/", $string))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

/**
 * @brief function checks if string is constant
 * @param $string, potentional constant
 * @return true if $string is constant
 * @return false if $string isnt constant
 */
    function isConst($string)
    {
        if(preg_match("/^nil@(nil|)$/",$string)
        || preg_match("/^bool@(true|false|)$/",$string)
        || preg_match("/^int@((\+|-|)[0-9]+|)$/",$string)
        || preg_match("/^string@([^\\\\]|\\\\0[0-2][0-9]|\\\\03[0-2]|\\\\035|\\\\092)*$/",$string) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

/**
 * @brief function checks if string is symbol
 * @param $string, potentional symbol (constant or variable)
 * @return true if $string is symbol
 * @return false if $string isnt symbol
 */
    function isSymb($string)
    {
        if(isConst($string) || isVar($string))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

/**
 * @brief function checks if string is label
 * @param $string, potentional label
 * @return true if $string is label
 * @return false if $string isnt label
 */
    function isLabel($string)
    {
        if(preg_match("/^[a-zA-Z_\-$&%*!?][a-zA-Z0-9_\-$&%*!?]*$/",$string))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

/**
 * @brief function checks if string is type
 * @param $string, potentional type
 * @return true if $string is type
 * @return false if $string isnt type
 */
    function isType($string)
    {
        if(preg_match("/^(int|bool|string|nil)$/",$string))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

/**
 * @brief function returns type of symbol
 * @param $string
 * @return trimmed $string
 */
    function getSymbType($string)
    {
        if(isVar($string))
        {
            return "var";
        }
        else
        {
            $string = preg_replace('/@.*/','', $string);
            return $string;
        }
    }

/**
 * @brief function returns value of symbol
 * @param $string
 * @return trimmed $string
 */
    function getSymbVal($string)
    {
        if(!isVar($string))
        {
            $string = preg_replace('/^[^@]*@/','', $string);
        }
            return $string;
    }

    //-----------------------------------------------main start---------------------------------------------------------
    $argError = argumentCheck();
    $statsFlag = false;
    if($argError == 2)  //if --stats was set
    {
        $statsFlag = true;
    }
    elseif($argError == 1 || $argError == $argumentFormatError)   //if wrong argument or help
    {
        if($argError == 1)  //help
        {
            $argError = 0;
        }
        exit($argError);
    }

    //main cycle
    $headerFlag = false;
    $opCounter = 0;

    //stats
    $commentCount = 0;
    $labelCount = 0;
    $labelArray = [];   //array with all labels
    $jumpArray = [];    //array with all jump labels
    $jumpCount = 0;
    $backJumpCount = 0;
    $badJumpCount = 0;
    $goodJumpCount = 0;
    $fileNameArray = [];    //to check if file was already opened before

    while($line = fgets(STDIN)) //takes line from input
    {
        $line = preg_replace('/\s+/',' ', $line);   //removes additional spaces
        $line = preg_replace('/^\s/','', $line);    //removes possible space at start of line
        if(preg_match("/#.*/", $line))  //counting comments for statistics
        {
            $commentCount++;
        }
        $line = preg_replace('/#.*/','', $line);    //removes comments
        $lineArray = explode(' ',trim($line));    //divide line into array

        if(!$headerFlag && $line != '') //if there was comment before header
        {
            if($lineArray[0] == ".IPPcode22" && count($lineArray) == 1)
            {
                $headerFlag = true;
                echo ("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
                echo ("<program language=\"IPPcode22\">\n");
                continue;   //skips rest of cycle
            }
            else
            {
                exit($headerError);
            }
        }

        //division by operation arguments
        switch (strtoupper($lineArray[0])) {
            // <nothing>
            case "CREATEFRAME":
            case "PUSHFRAME":
            case "POPFRAME":
            case "RETURN":
            case "BREAK":
                $opCounter++;
                if(count($lineArray) == 1)
                {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <var>
            case "DEFVAR":
            case "POPS" :
                $opCounter++;
                if (count($lineArray) == 2 && isVar($lineArray[1]))
                {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"var\">" . $lineArray[1] . "</arg1>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <symb>
            case "PUSHS":
            case "WRITE":
            case "EXIT":
            case "DPRINT":
                $opCounter++;
                if (count($lineArray) == 2 && isSymb($lineArray[1]))
                {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"" . getSymbType($lineArray[1]) . "\">" . getSymbVal($lineArray[1]) . "</arg1>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <label>
            case "CALL":
            case "LABEL":
            case "JUMP":
                $opCounter++;
                if (count($lineArray) == 2 && isLabel($lineArray[1]))
                {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"label\">" . $lineArray[1] . "</arg1>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                //stats
                if(strtoupper($lineArray[0]) == "LABEL")
                {
                    array_push($labelArray, $lineArray[1]);
                }
                else
                {
                    if(in_array($lineArray[1], $labelArray, true))   //if label already exists in label array
                    {
                        $backJumpCount++;
                    }
                    array_push($jumpArray, $lineArray[1]);
                    $jumpCount++;
                }
                break;

            // <var> <symb>
            case "MOVE":
            case "INT2CHAR":
            case "STRLEN":
            case "TYPE":
                $opCounter++;
                if (count($lineArray) == 3 && isVar($lineArray[1]) && isSymb($lineArray[2])) {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"var\">" . $lineArray[1] . "</arg1>\n");
                    echo("        <arg2 type=\"" . getSymbType($lineArray[2]) . "\">" . getSymbVal($lineArray[2]) . "</arg2>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <var> <type>
            case "READ":
                $opCounter++;
                if (count($lineArray) == 3 && isVar($lineArray[1]) && isType($lineArray[2])) {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"var\">" . $lineArray[1] . "</arg1>\n");
                    echo("        <arg2 type=\"type\">" . $lineArray[2] . "</arg2>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <var> <symb1> <symb2>
            case "ADD":
            case "SUB":
            case "MUL":
            case "IDIV":
            case "LT":
            case "GT":
            case "EQ":
            case "AND":
            case "OR":
            case "NOT":
            case "STRI2INT":
            case "CONCAT":
            case "GETCHAR":
            case "SETCHAR":
                $opCounter++;
                if (count($lineArray) == 4 && isVar($lineArray[1]) && isSymb($lineArray[2]) && isSymb($lineArray[3])) {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"var\">" . $lineArray[1] . "</arg1>\n");
                    echo("        <arg2 type=\"" . getSymbType($lineArray[2]) . "\">" . getSymbVal($lineArray[2]) . "</arg2>\n");
                    echo("        <arg3 type=\"" . getSymbType($lineArray[3]) . "\">" . getSymbVal($lineArray[3]) . "</arg3>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                break;

            // <label> <symb1> <symb2>
            case "JUMPIFEQ":
            case "JUMPIFNEQ":
                $opCounter++;
                if (count($lineArray) == 4 && isLabel($lineArray[1]) && isSymb($lineArray[2]) && isSymb($lineArray[3])) {
                    echo("    <instruction order=\"" . $opCounter . "\" opcode=\"" . $lineArray[0] . "\">\n");
                    echo("        <arg1 type=\"label\">" . $lineArray[1] . "</arg1>\n");
                    echo("        <arg2 type=\"" . getSymbType($lineArray[2]) . "\">" . getSymbVal($lineArray[2]) . "</arg2>\n");
                    echo("        <arg3 type=\"" . getSymbType($lineArray[3]) . "\">" . getSymbVal($lineArray[3]) . "</arg3>\n");
                    echo("    </instruction>\n");
                }
                else
                {
                    exit($otherOpCodeError);
                }
                //stats
                if(in_array($lineArray[1], $labelArray, true))   //if label already exists in label array
                {
                    $backJumpCount++;
                }
                array_push($jumpArray, $lineArray[1]);
                $jumpCount++;
                break;

            // unknown operation
            default:
                if ($lineArray[0] != '') //deleted comment line
                {
                    exit($opCodeError);
                }
                break;
        }
    }
    echo "</program>\n";

    if($statsFlag)  //if stats argument was set
    {
        $fp = NULL;
        for($i = 1; $i < $argc; $i++)
        {
            if(!strncmp($argv[$i], "--stats=", 8))
            {
                if($fp != NULL) //for closing already opened file
                {
                    fclose($fp);
                }
                $fileName = preg_replace('/^--stats=/','', $argv[$i]); //removes --stats and opens file
                if(in_array($fileName, $fileNameArray, true))   //if same output file is set more than once
                {
                    exit($outputError);
                }
                else
                {
                    array_push($fileNameArray, $fileName);
                }
                $fp = fopen($fileName, 'w');
            }
            else
            {
                switch ($argv[$i])  // division by argument type
                {
                    case "--comments" :
                        fwrite($fp, strval($commentCount) . "\n");
                        break;

                    case "--labels" :
                        fwrite($fp, count(array_unique($labelArray)) . "\n");
                        break;

                    case "--jumps" :
                        fwrite($fp, strval($jumpCount) . "\n");
                        break;

                    case "--fwjumps" :
                        foreach (array_unique($labelArray) as $label)
                        {
                            foreach ($jumpArray as $jump)
                            {
                                if($jump == $label) $goodJumpCount++;
                            }
                        }
                        $badJumpCount = $jumpCount - $goodJumpCount;
                        $fwJumpCount = $jumpCount - ($backJumpCount + $badJumpCount);
                        fwrite($fp, strval($fwJumpCount) . "\n");
                        $goodJumpCount = 0; //reset for next instruction
                        break;

                    case "--backjumps" :
                        fwrite($fp, strval($backJumpCount) . "\n");
                        break;

                    case "--badjumps" :
                        foreach (array_unique($labelArray) as $label)
                        {
                            foreach ($jumpArray as $jump)
                            {
                                if($jump == $label) $goodJumpCount++;   //if jump is in labels
                            }
                        }
                        $badJumpCount = $jumpCount - $goodJumpCount;
                        fwrite($fp, $badJumpCount . "\n");
                        $goodJumpCount = 0; //reset for next instruction
                        break;

                    case "--loc":
                        fwrite($fp, strval($opCounter) . "\n");
                        break;

                    default :
                        fclose($fp);
                        exit($argumentFormatError);
                        break;
                }
            }
        }
        fclose($fp);
    }
    exit(0);
?>