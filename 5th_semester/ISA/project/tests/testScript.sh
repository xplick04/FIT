#! /bin/sh

success=0

    #echo "Invalid arguments Tests";
	  $PWD/feedreader https://www.google.cz -b 2>/dev/null 1>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
        	echo "  Test 1 - unknown argument did not pass"
    fi;

    $PWD/feedreader https://www.google.cz -f url.txt 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 2 - conflicting feedfile and url did not pass"
    fi

    $PWD/feedreader -Tau 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 3 - no url source did not pass"
    fi

    $PWD/feedreader https://www.google.cz https://www.google.cz 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 4 - too many url sources did not pass"
    fi

    $PWD/feedreader -f $PWD/tests/invalidArguments/something.txt 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
        echo "  Test 5 - non existing feedfile did not pass"
    fi

    echo "Invalid arguments tests: $success/5 passed"
    success=0

    #echo "Invalid url tests";
    $PWD/feedreader htt://www.aksdk.cz 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
      echo "  Test 1 - invalid url did not pass"
    fi

    $PWD/feedreader www.google.com 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 2 - invalid url did not pass"
    fi

    $PWD/feedreader $PWD/tests/invalidURL/invaliURL.txt 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 3 - no valid url in feedfile did not pass"
    fi

    echo "Invalid url tests: $success/3 passed"
    success=0

    $PWD/feedreader https://www.seznam.cz 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 1 - invalid xml from argument did not pass"
    fi

    $PWD/feedreader -f $PWD/tests/invalidXML/invalidXML.txt 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 2 - invalid xml from feedfile did not pass"
    fi

    echo "Invalid XML tests: $success/2 passed"
    success=0

    $PWD/feedreader  https://www.fit.vut.cz:443/fit/news-rss/ -c $PWD/tests/truststore.pem 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 1 - non existing certificates file did not pass"
    fi

      $PWD/feedreader  https://www.fit.vut.cz:443/fit/news-rss/ -C $PWD/something 2>/dev/null
      if [ $? -eq 1 ];
      then
        success=$((success + 1))
      else
            echo "  Test 1 - non existing certificates directory did not pass"
      fi

    $PWD/feedreader 2>/dev/null
    if [ $? -eq 1 ];
    then
      success=$((success + 1))
    else
          echo "  Test 2 - invalid certificates for page did not pass"
    fi

    echo "Invalid certificates tests: $success/3 passed"
    success=0

#-------------------------------------------------------------------------------------------------------
    $PWD/feedreader https://xkcd.com/atom.xml 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
        echo "  Test 1 - valid address did not pass";
    fi

    $PWD/feedreader https://xkcd.com:443/atom.xml >/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
        echo "  Test 2 - valid address with port did not pass";
    fi

    $PWD/feedreader -f $PWD/tests/validAtom/validAtom.txt 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
        echo "  Test 3 - valid address from feedfile did not pass";
    fi

    $PWD/feedreader https://xkcd.com/atom.xml -Tau 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
      echo "  Test 4 - valid address with flags did not pass";
    fi

    echo "Valid Atom tests $success/4 passed";
    success=0;

    $PWD/feedreader https://www.fit.vut.cz:443/fit/news-rss/ 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
       echo "  Test 1 - valid address did not pass";
    fi

    $PWD/feedreader https://www.fit.vut.cz:443/fit/news-rss/ -Tau 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
        echo "  Test 2 - valid address with flags did not pass";
    fi

    echo "Valid RSS tests $success/2 passed";
    success=0;

    $PWD/feedreader -f $PWD/tests/validAtomRSS/validAtomRSS.txt 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
          echo "  Test 1 - valid RSS and Atom addresses from feedfile did not pass"
    fi

    $PWD/feedreader -f $PWD/tests/validAtomRSS/validAtomRSS.txt -Tau 1>/dev/null
    if [ $? -eq 0 ];
    then
      success=$((success + 1))
    else
          echo "  Test 2 - valid RSS and Atom addresses with flags did not pass"
    fi

    echo "Valid Atom and RSS tests : $success/2 passed";
