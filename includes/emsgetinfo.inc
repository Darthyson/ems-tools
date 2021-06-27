<?php
	require_once("/emsincludes/emsqry.inc");

	function getWWInfo($src){
	  return getInfo($src,"ww requestdata");
	}

	function getPartyInfo($src){
	  return getInfo($src,"hk2 requestdata");
	}

	function getHKInfo($src){
	  return getInfo($src,"hk2 requestdata");
	}

	function getRCInfo($src){
	  return getInfo($src,"rc requestdata");
	}

	function getUBAInfo($src){
	  return getInfo($src,"uba requestdata");
	}

	function getMaintenanceInfo($src){
	  return getInfo($src,"uba requestdata");
	}

	function getCurrentMaintenanceInfo($src){
	  return getInfoCommand($src,"uba checkmaintenanceneeded");
	}

	function getInfo($src,$command){
	  global $cache;
	  if (!isset($cache)) $cache=array();
	  # Warmwasser
	  $srch = array("wwmode" => 	"ww opmode =",
					"zirmode" => 	"zirkpump opmode =",
					"spzir" => 	"zirkpump switchpoints =",
					"wwtag" => 	"ww settemperature =",
					"wwactive" => 	"ww masterswitch =",
					"limittemp" => 	"ww maxtemperature =",
					"loadled" => 	"ww onetimeloadindicator =",
					"tdstat" => 	"ww desinfection =",
					"tdday" => 	"ww desinfectionday =",
					"tdhour" => 	"ww desinfectionhour =",
					"tdtemp" => 	"ww desinfectiontemperature =",
	  # Heizkreis
					"day" => 		"hk2 daytemperature =",
					"night" => 		"hk2 nighttemperature =",
					"tagbetr" =>		"hk2 daymode =",
					"mode" =>		"hk2 opmode =",
					"frost" => 		"hk2 frostprotecttemperature =",
					"temptemp" => 		"hk2 temperatureoverride =",
					"nachtred" => 		"hk2 reducedmodethreshold =",
					"urlaubnachtred" => 	"hk2 vacationreducedmodethreshold =",
					"absquit"  =>	 	"hk2 cancelreducedmodethreshold =",
					"urlaubt"  =>	 	"hk2 vacationtemperature =",
					"summertime" => 	"hk2 summerwinterthreshold =",
					"auslegtemp" => 	"hk2 designtemperature =",
					"minvorlauf" => 	"hk2 mintemperature =",
					"maxvorlauf" => 	"hk2 maxtemperature =",
					"redmode" => 		"hk2 reductionmode =",
					"refinput" => 		"hk2 relevantparameter =",
					"refinputvac" => 	"hk2 vacationreductionmode =",
					"maxroomeffect" => 	"hk2 maxroomeffect =",
					"schedoptimizer" => 	"hk2 scheduleoptimizer =",
					"frostmode" => 		"hk2 frostprotectmode =",
					"raumoffset" => 	"hk2 roomtemperatureoffset =",
	  # RC35
					"minaussentemp" => 	"rc mintemperature =",
					"gebaeude" => 		"rc buildingtype =",
					"daempfung" => 		"rc damping =",
	  # UBA
					"antipen" => 		"antipendelminutes =",
					"heizactive" => 	"heater masterswitch =",
					"kpmin" => 		"heaterpump minmodulation =",
					"kpmax"  => 		"heaterpump maxmodulation =",
					"kpnachl"  => 		"heaterpump followupminutes =",
					"burnmin" => 		"burner minmodulation =",
					"burnmax"  => 		"burner maxmodulation =",
					"hystein"  => 		"heater onhysteresis =",
					"hystaus"  => 		"heater offhysteresis =",
					"totalhours"  => 	"operatingminutes =",
	  # Party
					"pause" => 		"hk2 pausehours =",
					"party" => 		"hk2 partyhours =",
	  # Wartung
					"mtmode" => 		"heater maintenancereminder =",
					"mthours" => 		"heater maintenanceintervalin100hours =",
					"mtdate"  => 		"heater maintenancedate = ",
					"mtactive"  => 		"heater maintenancedue =",
	  );

	  if (isset($cache[$command])){
		$res = $cache[$command];
	  } else {
		#print('getInfo.$command: '.$command);
		#print('<p></p>');
		$res=doEmsDataCommand($command);
		#foreach ($res as $i){
			#print('getInfo.$res: '.$i);
			#print('<p></p>');
		#}
		$cache[$command]=$res;
	  }

	  $searchstr = $srch[$src];

	  foreach($res as $r){
		$r = utf8_decode($r);
		$r2 = str_replace($searchstr,"",$r);
		if ($r2 != $r) {
		  # Richtige Zeile gefunden!
		  $ra = explode("|",$r2);
		  switch($src){
			case "mtdate":
			  $ra[0] = flipdater($ra[0]);
			  break;
		  }
		  return trim($ra[0]);
		}
	  }
	}

	function flipdater($s){
	  if ($s=="") $s = "2000-01-01";
	  $s = str_replace("-",".",$s);
	  $sa = explode(".",$s);
	  $day = substr("00".trim($sa[2]),-2);
	  $month = substr("00".$sa[1],-2);
	  $year = "2".substr("000".$sa[0],-3);
	  $res = ($day.".".$month.".".$year);
	  return $res;
	}

	function getHolVacInfo($src){
	  $res = doEmsCommand("hk1 get".$src);  
	  $out = array();
	  foreach($res as $l){
		if (substr($l,0,5)=="begin") $out["von"] = flipdater(substr($l,6,10));
		if (substr($l,0,3)=="end") $out["bis"] = flipdater(substr($l,4,10));
	  }
	  return $out;
	}

	function getContactInfo($src){
	  $res = doEmsCommand("rc getcontactinfo ");  
	  $out = array();
	  foreach($res as $l){
		if ($l!="OK\n") $out[] = trim($l);
		if ($l="OK\n") $out[1] = trim($l);
	  }
	  return $out[$src-1];;  
	}
?>
