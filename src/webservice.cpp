/*
 Name:		ESP32APRS T-TWR Plus
 Created:	13-10-2023 14:27:23
 Author:	HS5TQA/Atten
 Github:	https://github.com/nakhonthai
 Facebook:	https://www.facebook.com/atten
 Support IS: host:aprs.dprns.com port:14580 or aprs.hs5tqa.ampr.org:14580
 Support IS monitor: http://aprs.dprns.com:14501 or http://aprs.hs5tqa.ampr.org:14501
*/
#include "AFSK.h"
#include "webservice.h"
#include "base64.hpp"
#include "wireguard_vpn.h"
#include <LibAPRSesp.h>
#include <parse_aprs.h>

// Web Server;
WebServer server(80);

String webString;

bool defaultSetting = false;

void serviceHandle()
{
	server.handleClient();
}

void handle_logout()
{
	webString = "Log out";
	server.send(401, "text/html", webString);
}

void setMainPage()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	webString = "<html><head>\n";
	webString += "<meta name=\"robots\" content=\"index\" />\n";
	webString += "<meta name=\"robots\" content=\"follow\" />\n";
	webString += "<meta name=\"language\" content=\"English\" />\n";
	webString += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
	webString += "<meta name=\"GENERATOR\" content=\"configure 20230924\" />\n";
	webString += "<meta name=\"Author\" content=\"Somkiat Nakhonthai (HS5TQA)\" />\n";
	webString += "<meta name=\"Description\" content=\"Web Embedded Configuration\" />\n";
	webString += "<meta name=\"KeyWords\" content=\"ESP32IGATE,APRS\" />\n";
	webString += "<meta http-equiv=\"Cache-Control\" content=\"no-cache, no-store, must-revalidate\" />\n";
	webString += "<meta http-equiv=\"pragma\" content=\"no-cache\" />\n";
	webString += "<link rel=\"shortcut icon\" href=\"http://aprs.nakhonthai.net/images/favicon.ico\" type=\"image/x-icon\" />\n";
	webString += "<meta http-equiv=\"Expires\" content=\"0\" />\n";
	webString += "<title>ESP32APRS_T-TWR</title>\n";
	webString += "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />\n";
	webString += "<script src=\"jquery-3.7.1.js\"></script>\n";
	webString += "<script type=\"text/javascript\">\n";
	webString += "function selectTab(evt, tabName) {\n";
	webString += "var i, tabcontent, tablinks;\n";
	webString += "tablinks = document.getElementsByClassName(\"nav-tabs\");\n";
	webString += "for (i = 0; i < tablinks.length; i++) {\n";
	webString += "tablinks[i].className = tablinks[i].className.replace(\" active\", \"\");\n";
	webString += "}\n";
	webString += "\n";
	webString += "//document.getElementById(tabName).style.display = \"block\";\n";
	webString += "if (tabName == 'DashBoard') {\n";
	webString += "$(\"#contentmain\").load(\"/dashboard\");\n";
	webString += "} else if (tabName == 'Radio') {\n";
	webString += "$(\"#contentmain\").load(\"/radio\");\n";
	webString += "} else if (tabName == 'IGATE') {\n";
	webString += "$(\"#contentmain\").load(\"/igate\");\n";
	webString += "} else if (tabName == 'DIGI') {\n";
	webString += "$(\"#contentmain\").load(\"/digi\");\n";
	webString += "} else if (tabName == 'TRACKER') {\n";
	webString += "$(\"#contentmain\").load(\"/tracker\");\n";
	webString += "} else if (tabName == 'VPN') {\n";
	webString += "$(\"#contentmain\").load(\"/vpn\");\n";
	webString += "} else if (tabName == 'Wireless') {\n";
	webString += "$(\"#contentmain\").load(\"/wireless\");\n";
	webString += "} else if (tabName == 'System') {\n";
	webString += "$(\"#contentmain\").load(\"/system\");\n";
	webString += "} else if (tabName == 'File') {\n";
	webString += "$(\"#contentmain\").load(\"/file\");\n";
	webString += "} else if (tabName == 'About') {\n";
	webString += "$(\"#contentmain\").load(\"/about\");\n";
	webString += "}\n";
	webString += "\n";
	webString += "if (evt != null) evt.currentTarget.className += \" active\";\n";
	webString += "}\n";
	webString += "</script>\n";
	webString += "</head>\n";
	webString += "\n";
	webString += "<body onload=\"selectTab(event, 'DashBoard')\">\n";
	webString += "\n";
	webString += "<div class=\"container\">\n";
	webString += "<div class=\"header\">\n";
	// webString += "<div style=\"font-size: 8px; text-align: right; padding-right: 8px;\">ESP32APRS T-TWR Plus Firmware V" + String(VERSION) + "</div>\n";
	// webString += "<div style=\"font-size: 8px; text-align: right; padding-right: 8px;\"><a href=\"/logout\">[LOG OUT]</a></div>\n";
	webString += "<h1>ESP32APRS T-TWR Plus</h1>\n";
	webString += "<div style=\"font-size: 8px; text-align: right; padding-right: 8px;\"><a href=\"/logout\">[LOG OUT]</a></div>\n";
	webString += "<div class=\"row\">\n";
	webString += "<ul class=\"nav nav-tabs\" style=\"margin: 5px;\">\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'DashBoard')\">DashBoard</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'Radio')\" id=\"btnRadio\">Radio</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'IGATE')\">IGATE</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'DIGI')\">DIGI</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'TRACKER')\">TRACKER</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'VPN')\">VPN</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'Wireless')\">Wireless</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'System')\">System</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'File')\">File</button>\n";
	webString += "<button class=\"nav-tabs\" onclick=\"selectTab(event, 'About')\">About</button>\n";
	webString += "</ul>\n";
	webString += "</div>\n";
	webString += "</div>\n";
	webString += "\n";

	webString += "<div class=\"contentwide\" id=\"contentmain\"  style=\"font-size: 2pt;\">\n";
	webString += "\n";
	webString += "</div>\n";
	webString += "<br />\n";
	webString += "<div class=\"footer\">\n";
	webString += "ESP32APRS Web Configuration<br />Copy right ©2023.\n";
	webString += "<br />\n";
	webString += "</div>\n";
	webString += "</div>\n";
	webString += "<!-- <script type=\"text/javascript\" src=\"/nice-select.min.js\"></script> -->\n";
	webString += "<script type=\"text/javascript\">\n";
	webString += "var selectize = document.querySelectorAll('select')\n";
	webString += "var options = { searchable: true };\n";
	webString += "selectize.forEach(function (select) {\n";
	webString += "if (select.length > 30 && null === select.onchange && !select.name.includes(\"ExtendedId\")) {\n";
	webString += "select.classList.add(\"small\", \"selectize\");\n";
	webString += "tabletd = select.closest('td');\n";
	webString += "tabletd.style.cssText = 'overflow-x:unset';\n";
	webString += "NiceSelect.bind(select, options);\n";
	webString += "}\n";
	webString += "});\n";
	webString += "</script>\n";
	webString += "</body>\n";
	webString += "</html>";
	server.send(200, "text/html", webString); // send to someones browser when asked
}

////////////////////////////////////////////////////////////
// handler for web server request: http://IpAddress/      //
////////////////////////////////////////////////////////////

void handle_css()
{
	String css = ".container{width:800px;text-align:left;margin:auto;border-radius:10px 10px 10px 10px;-moz-border-radius:10px 10px 10px 10px;-webkit-border-radius:10px 10px 10px 10px;-khtml-border-radius:10px 10px 10px 10px;-ms-border-radius:10px 10px 10px 10px;box-shadow:3px 3px 3px #707070;background:#fff;border-color: #2194ec;padding: 0px;border-width: 5px;border-style:solid;}body,font{font:12px verdana,arial,sans-serif;color:#fff}.header{background:#2194ec;text-decoration:none;color:#fff;font-family:verdana,arial,sans-serif;text-align:left;padding:5px 0;border-radius:10px 10px 0 0;-moz-border-radius:10px 10px 0 0;-webkit-border-radius:10px 10px 0 0;-khtml-border-radius:10px 10px 0 0;-ms-border-radius:10px 10px 0 0}.content{margin:0 0 0 166px;padding:1px 5px 5px;color:#000;background:#fff;text-align:center;font-size: 8pt;}.contentwide{padding:50px 5px 5px;color:#000;background:#fff;text-align:center}.contentwide h2{color:#000;font:1em verdana,arial,sans-serif;text-align:center;font-weight:700;padding:0;margin:0;font-size: 12pt;}.footer{background:#2194ec;text-decoration:none;color:#fff;font-family:verdana,arial,sans-serif;font-size:9px;text-align:center;padding:10px 0;border-radius:0 0 10px 10px;-moz-border-radius:0 0 10px 10px;-webkit-border-radius:0 0 10px 10px;-khtml-border-radius:0 0 10px 10px;-ms-border-radius:0 0 10px 10px;clear:both}#tail{height:450px;width:805px;overflow-y:scroll;overflow-x:scroll;color:#0f0;background:#000}table{vertical-align:middle;text-align:center;empty-cells:show;padding-left:3;padding-right:3;padding-top:3;padding-bottom:3;border-collapse:collapse;border-color:#0f07f2;border-style:solid;border-spacing:0px;border-width:3px;text-decoration:none;color:#fff;background:#000;font-family:verdana,arial,sans-serif;font-size : 12px;width:100%;white-space:nowrap}table th{font-size: 10pt;font-family:lucidia console,Monaco,monospace;text-shadow:1px 1px #0e038c;text-decoration:none;background:#0525f7;border:1px solid silver}table tr:nth-child(even){background:#f7f7f7}table tr:nth-child(odd){background:#eeeeee}table td{color:#000;font-family:lucidia console,Monaco,monospace;text-decoration:none;border:1px solid #010369}body{background:#edf0f5;color:#000}a{text-decoration:none}a:link,a:visited{text-decoration:none;color:#0000e0;font-weight:400}th:last-child a.tooltip:hover span{left:auto;right:0}ul{padding:5px;margin:10px 0;list-style:none;float:left}ul li{float:left;display:inline;margin:0 10px}ul li a{text-decoration:none;float:left;color:#999;cursor:pointer;font:900 14px/22px arial,Helvetica,sans-serif}ul li a span{margin:0 10px 0 -10px;padding:1px 8px 5px 18px;position:relative;float:left}h1{text-shadow:2px 2px #303030;text-align:center}.toggle{position:absolute;margin-left:-9999px;visibility:hidden}.toggle+label{display:block;position:relative;cursor:pointer;outline:none}input.toggle-round-flat+label{padding:1px;width:33px;height:18px;background-color:#ddd;border-radius:10px;transition:background .4s}input.toggle-round-flat+label:before,input.toggle-round-flat+label:after{display:block;position:absolute;}input.toggle-round-flat+label:before{top:1px;left:1px;bottom:1px;right:1px;background-color:#fff;border-radius:10px;transition:background .4s}input.toggle-round-flat+label:after{top:2px;left:2px;bottom:2px;width:16px;background-color:#ddd;border-radius:12px;transition:margin .4s,background .4s}input.toggle-round-flat:checked+label{background-color:#dd4b39}input.toggle-round-flat:checked+label:after{margin-left:14px;background-color:#dd4b39}@-moz-document url-prefix(){select,input{margin:0;padding:0;border-width:1px;font:12px verdana,arial,sans-serif}input[type=button],button,input[type=submit]{padding:0 3px;border-radius:3px 3px 3px 3px;-moz-border-radius:3px 3px 3px 3px}}.nice-select.small,.nice-select-dropdown li.option{height:24px!important;min-height:24px!important;line-height:24px!important}.nice-select.small ul li:nth-of-type(2){clear:both}.nav{margin-bottom:0;padding-left:10;list-style:none}.nav>li{position:relative;display:block}.nav>li>a{position:relative;display:block;padding:5px 10px}.nav>li>a:hover,.nav>li>a:focus{text-decoration:none;background-color:#eee}.nav>li.disabled>a{color:#999}.nav>li.disabled>a:hover,.nav>li.disabled>a:focus{color:#999;text-decoration:none;background-color:initial;cursor:not-allowed}.nav .open>a,.nav .open>a:hover,.nav .open>a:focus{background-color:#eee;border-color:#428bca}.nav .nav-divider{height:1px;margin:9px 0;overflow:hidden;background-color:#e5e5e5}.nav>li>a>img{max-width:none}.nav-tabs{border-bottom:1px solid #ddd}.nav-tabs>li{float:left;margin-bottom:-1px}.nav-tabs>li>a{margin-right:0;line-height:1.42857143;border:1px solid #ddd;border-radius:10px 10px 0 0}.nav-tabs>li>a:hover{border-color:#eee #eee #ddd}.nav-tabs>button{margin-right:0;line-height:1.42857143;border:2px solid #ddd;border-radius:10px 10px 0 0}.nav-tabs>button:hover{background-color:#25bbfc;border-color:#428bca;color:#eaf2f9;border-bottom-color:transparent;}.nav-tabs>button.active,.nav-tabs>button.active:hover,.nav-tabs>button.active:focus{color:#f7fdfd;background-color:#1aae0d;border:1px solid #ddd;border-bottom-color:transparent;cursor:default}.nav-tabs>li.active>a,.nav-tabs>li.active>a:hover,.nav-tabs>li.active>a:focus{color:#428bca;background-color:#e5e5e5;border:1px solid #ddd;border-bottom-color:transparent;cursor:default}.nav-tabs.nav-justified{width:100%;border-bottom:0}.nav-tabs.nav-justified>li{float:none}.nav-tabs.nav-justified>li>a{text-align:center;margin-bottom:5px}.nav-tabs.nav-justified>.dropdown .dropdown-menu{top:auto;left:auto}.nav-status{float:left;margin:0;padding:3px;width:160px;font-weight:400;min-height:600}#bar,#prgbar {background-color: #f1f1f1;border-radius: 14px}#bar {background-color: #3498db;width: 0%;height: 14px}.switch{position:relative;display:inline-block;width:34px;height:16px}.switch input{opacity:0;width:0;height:0}.slider{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background-color:#f55959;-webkit-transition:.4s;transition:.4s}.slider:before{position:absolute;content:\"\";height:12px;width:12px;left:2px;bottom:2px;background-color:#fff;-webkit-transition:.4s;transition:.4s}input:checked+.slider{background-color:#5ca30a}input:focus+.slider{box-shadow:0 0 1px #5ca30a}input:checked+.slider:before{-webkit-transform:translateX(16px);-ms-transform:translateX(16px);transform:translateX(16px)}.slider.round{border-radius:34px}.slider.round:before{border-radius:50%}\n";
	server.send(200, "text/css", css); // send to someones browser when asked
}

void handle_jquery()
{
	String css = "!function(e,t){\"use strict\";\"object\"==typeof module&&\"object\"==typeof module.exports?module.exports=e.document?t(e,!0):function(e){if(!e.document)throw new Error(\"jQuery requires a window with a document\");return t(e)}:t(e)}(\"undefined\"!=typeof window?window:this,function(ie,e){\"use strict\";var oe=[],r=Object.getPrototypeOf,ae=oe.slice,g=oe.flat?function(e){return oe.flat.call(e)}:function(e){return oe.concat.apply([],e)},s=oe.push,se=oe.indexOf,n={},i=n.toString,ue=n.hasOwnProperty,o=ue.toString,a=o.call(Object),le={},v=function(e){return\"function\"==typeof e&&\"number\"!=typeof e.nodeType&&\"function\"!=typeof e.item},y=function(e){return null!=e&&e===e.window},C=ie.document,u={type:!0,src:!0,nonce:!0,noModule:!0};function m(e,t,n){var r,i,o=(n=n||C).createElement(\"script\");if(o.text=e,t)for(r in u)(i=t[r]||t.getAttribute&&t.getAttribute(r))&&o.setAttribute(r,i);n.head.appendChild(o).parentNode.removeChild(o)}function x(e){return null==e?e+\"\":\"object\"==typeof e||\"function\"==typeof e?n[i.call(e)]||\"object\":typeof e}var t=\"3.7.1\",l=/HTML$/i,ce=function(e,t){return new ce.fn.init(e,t)};function c(e){var t=!!e&&\"length\"in e&&e.length,n=x(e);return!v(e)&&!y(e)&&(\"array\"===n||0===t||\"number\"==typeof t&&0<t&&t-1 in e)}function fe(e,t){return e.nodeName&&e.nodeName.toLowerCase()===t.toLowerCase()}ce.fn=ce.prototype={jquery:t,constructor:ce,length:0,toArray:function(){return ae.call(this)},get:function(e){return null==e?ae.call(this):e<0?this[e+this.length]:this[e]},pushStack:function(e){var t=ce.merge(this.constructor(),e);return t.prevObject=this,t},each:function(e){return ce.each(this,e)},map:function(n){return this.pushStack(ce.map(this,function(e,t){return n.call(e,t,e)}))},slice:function(){return this.pushStack(ae.apply(this,arguments))},first:function(){return this.eq(0)},last:function(){return this.eq(-1)},even:function(){return this.pushStack(ce.grep(this,function(e,t){return(t+1)%2}))},odd:function(){return this.pushStack(ce.grep(this,function(e,t){return t%2}))},eq:function(e){var t=this.length,n=+e+(e<0?t:0);return this.pushStack(0<=n&&n<t?[this[n]]:[])},end:function(){return this.prevObject||this.constructor()},push:s,sort:oe.sort,splice:oe.splice},ce.extend=ce.fn.extend=function(){var e,t,n,r,i,o,a=arguments[0]||{},s=1,u=arguments.length,l=!1;for(\"boolean\"==typeof a&&(l=a,a=arguments[s]||{},s++),\"object\"==typeof a||v(a)||(a={}),s===u&&(a=this,s--);s<u;s++)if(null!=(e=arguments[s]))for(t in e)r=e[t],\"__proto__\"!==t&&a!==r&&(l&&r&&(ce.isPlainObject(r)||(i=Array.isArray(r)))?(n=a[t],o=i&&!Array.isArray(n)?[]:i||ce.isPlainObject(n)?n:{},i=!1,a[t]=ce.extend(l,o,r)):void 0!==r&&(a[t]=r));return a},ce.extend({expando:\"jQuery\"+(t+Math.random()).replace(/\\D/g,\"\"),isReady:!0,error:function(e){throw new Error(e)},noop:function(){},isPlainObject:function(e){var t,n;return!(!e||\"[object Object]\"!==i.call(e))&&(!(t=r(e))||\"function\"==typeof(n=ue.call(t,\"constructor\")&&t.constructor)&&o.call(n)===a)},isEmptyObject:function(e){var t;for(t in e)return!1;return!0},globalEval:function(e,t,n){m(e,{nonce:t&&t.nonce},n)},each:function(e,t){var n,r=0;if(c(e)){for(n=e.length;r<n;r++)if(!1===t.call(e[r],r,e[r]))break}else for(r in e)if(!1===t.call(e[r],r,e[r]))break;return e},text:function(e){var t,n=\"\",r=0,i=e.nodeType;if(!i)while(t=e[r++])n+=ce.text(t);return 1===i||11===i?e.textContent:9===i?e.documentElement.textContent:3===i||4===i?e.nodeValue:n},makeArray:function(e,t){var n=t||[];return null!=e&&(c(Object(e))?ce.merge(n,\"string\"==typeof e?[e]:e):s.call(n,e)),n},inArray:function(e,t,n){return null==t?-1:se.call(t,e,n)},isXMLDoc:function(e){var t=e&&e.namespaceURI,n=e&&(e.ownerDocument||e).documentElement;return!l.test(t||n&&n.nodeName||\"HTML\")},merge:function(e,t){for(var n=+t.length,r=0,i=e.length;r<n;r++)e[i++]=t[r];return e.length=i,e},grep:function(e,t,n){for(var r=[],i=0,o=e.length,a=!n;i<o;i++)!t(e[i],i)!==a&&r.push(e[i]);return r},map:function(e,t,n){var r,i,o=0,a=[];if(c(e))for(r=e.length;o<r;o++)null!=(i=t(e[o],o,n))&&a.push(i);else for(o in e)null!=(i=t(e[o],o,n))&&a.push(i);return g(a)},guid:1,support:le}),\"function\"==typeof Symbol&&(ce.fn[Symbol.iterator]=oe[Symbol.iterator]),ce.each(\"Boolean Number String Function Array Date RegExp Object Error Symbol\".split(\" \"),function(e,t){n[\"[object \"+t+\"]\"]=t.toLowerCase()});var pe=oe.pop,de=oe.sort,he=oe.splice,ge=\"[\\\\x20\\\\t\\\\r\\\\n\\\\f]\",ve=new RegExp(\"^\"+ge+\"+|((?:^|[^\\\\\\\\])(?:\\\\\\\\.)*)\"+ge+\"+$\",\"g\");ce.contains=function(e,t){var n=t&&t.parentNode;return e===n||!(!n||1!==n.nodeType||!(e.contains?e.contains(n):e.compareDocumentPosition&&16&e.compareDocumentPosition(n)))};var f=/([\\0-\\x1f\\x7f]|^-?\\d)|^-$|[^\\x80-\\uFFFF\\w-]/g;function p(e,t){return t?\"\\0\"===e?\"\\ufffd\":e.slice(0,-1)+\"\\\\\"+e.charCodeAt(e.length-1).toString(16)+\" \":\"\\\\\"+e}ce.escapeSelector=function(e){return(e+\"\").replace(f,p)};var ye=C,me=s;!function(){var e,b,w,o,a,T,r,C,d,i,k=me,S=ce.expando,E=0,n=0,s=W(),c=W(),u=W(),h=W(),l=function(e,t){return e===t&&(a=!0),0},f=\"checked|selected|async|autofocus|autoplay|controls|defer|disabled|hidden|ismap|loop|multiple|open|readonly|required|scoped\",t=\"(?:\\\\\\\\[\\\\da-fA-F]{1,6}\"+ge+\"?|\\\\\\\\[^\\\\r\\\\n\\\\f]|[\\\\w-]|[^\\0-\\\\x7f])+\",p=\"\\\\[\"+ge+\"*(\"+t+\")(?:\"+ge+\"*([*^$|!~]?=)\"+ge+\"*(?:'((?:\\\\\\\\.|[^\\\\\\\\'])*)'|\\\"((?:\\\\\\\\.|[^\\\\\\\\\\\"])*)\\\"|(\"+t+\"))|)\"+ge+\"*\\\\]\",g=\":(\"+t+\")(?:\\\\((('((?:\\\\\\\\.|[^\\\\\\\\'])*)'|\\\"((?:\\\\\\\\.|[^\\\\\\\\\\\"])*)\\\")|((?:\\\\\\\\.|[^\\\\\\\\()[\\\\]]|\"+p+\")*)|.*)\\\\)|)\",v=new RegExp(ge+\"+\",\"g\"),y=new RegExp(\"^\"+ge+\"*,\"+ge+\"*\"),m=new RegExp(\"^\"+ge+\"*([>+~]|\"+ge+\")\"+ge+\"*\"),x=new RegExp(ge+\"|>\"),j=new RegExp(g),A=new RegExp(\"^\"+t+\"$\"),D={ID:new RegExp(\"^#(\"+t+\")\"),CLASS:new RegExp(\"^\\\\.(\"+t+\")\"),TAG:new RegExp(\"^(\"+t+\"|[*])\"),ATTR:new RegExp(\"^\"+p),PSEUDO:new RegExp(\"^\"+g),CHILD:new RegExp(\"^:(only|first|last|nth|nth-last)-(child|of-type)(?:\\\\(\"+ge+\"*(even|odd|(([+-]|)(\\\\d*)n|)\"+ge+\"*(?:([+-]|)\"+ge+\"*(\\\\d+)|))\"+ge+\"*\\\\)|)\",\"i\"),bool:new RegExp(\"^(?:\"+f+\")$\",\"i\"),needsContext:new RegExp(\"^\"+ge+\"*[>+~]|:(even|odd|eq|gt|lt|nth|first|last)(?:\\\\(\"+ge+\"*((?:-\\\\d)?\\\\d*)\"+ge+\"*\\\\)|)(?=[^-]|$)\",\"i\")},N=/^(?:input|select|textarea|button)$/i,q=/^h\\d$/i,L=/^(?:#([\\w-]+)|(\\w+)|\\.([\\w-]+))$/,H=/[+~]/,O=new RegExp(\"\\\\\\\\[\\\\da-fA-F]{1,6}\"+ge+\"?|\\\\\\\\([^\\\\r\\\\n\\\\f])\",\"g\"),P=function(e,t){var n=\"0x\"+e.slice(1)-65536;return t||(n<0?String.fromCharCode(n+65536):String.fromCharCode(n>>10|55296,1023&n|56320))},M=function(){V()},R=J(function(e){return!0===e.disabled&&fe(e,\"fieldset\")},{dir:\"parentNode\",next:\"legend\"});try{k.apply(oe=ae.call(ye.childNodes),ye.childNodes),oe[ye.childNodes.length].nodeType}catch(e){k={apply:function(e,t){me.apply(e,ae.call(t))},call:function(e){me.apply(e,ae.call(arguments,1))}}}function I(t,e,n,r){var i,o,a,s,u,l,c,f=e&&e.ownerDocument,p=e?e.nodeType:9;if(n=n||[],\"string\"!=typeof t||!t||1!==p&&9!==p&&11!==p)return n;if(!r&&(V(e),e=e||T,C)){if(11!==p&&(u=L.exec(t)))if(i=u[1]){if(9===p){if(!(a=e.getElementById(i)))return n;if(a.id===i)return k.call(n,a),n}else if(f&&(a=f.getElementById(i))&&I.contains(e,a)&&a.id===i)return k.call(n,a),n}else{if(u[2])return k.apply(n,e.getElementsByTagName(t)),n;if((i=u[3])&&e.getElementsByClassName)return k.apply(n,e.getElementsByClassName(i)),n}if(!(h[t+\" \"]||d&&d.test(t))){if(c=t,f=e,1===p&&(x.test(t)||m.test(t))){(f=H.test(t)&&U(e.parentNode)||e)==e&&le.scope||((s=e.getAttribute(\"id\"))?s=ce.escapeSelector(s):e.setAttribute(\"id\",s=S)),o=(l=Y(t)).length;while(o--)l[o]=(s?\"#\"+s:\":scope\")+\" \"+Q(l[o]);c=l.join(\",\")}try{return k.apply(n,f.querySelectorAll(c)),n}catch(e){h(t,!0)}finally{s===S&&e.removeAttribute(\"id\")}}}return re(t.replace(ve,\"$1\"),e,n,r)}function W(){var r=[];return function e(t,n){return r.push(t+\" \")>b.cacheLength&&delete e[r.shift()],e[t+\" \"]=n}}function F(e){return e[S]=!0,e}function $(e){var t=T.createElement(\"fieldset\");try{return!!e(t)}catch(e){return!1}finally{t.parentNode&&t.parentNode.removeChild(t),t=null}}function B(t){return function(e){return fe(e,\"input\")&&e.type===t}}function _(t){return function(e){return(fe(e,\"input\")||fe(e,\"button\"))&&e.type===t}}function z(t){return function(e){return\"form\"in e?e.parentNode&&!1===e.disabled?\"label\"in e?\"label\"in e.parentNode?e.parentNode.disabled===t:e.disabled===t:e.isDisabled===t||e.isDisabled!==!t&&R(e)===t:e.disabled===t:\"label\"in e&&e.disabled===t}}function X(a){return F(function(o){return o=+o,F(function(e,t){var n,r=a([],e.length,o),i=r.length;while(i--)e[n=r[i]]&&(e[n]=!(t[n]=e[n]))})})}function U(e){return e&&\"undefined\"!=typeof e.getElementsByTagName&&e}function V(e){var t,n=e?e.ownerDocument||e:ye;return n!=T&&9===n.nodeType&&n.documentElement&&(r=(T=n).documentElement,C=!ce.isXMLDoc(T),i=r.matches||r.webkitMatchesSelector||r.msMatchesSelector,r.msMatchesSelector&&ye!=T&&(t=T.defaultView)&&t.top!==t&&t.addEventListener(\"unload\",M),le.getById=$(function(e){return r.appendChild(e).id=ce.expando,!T.getElementsByName||!T.getElementsByName(ce.expando).length}),le.disconnectedMatch=$(function(e){return i.call(e,\"*\")}),le.scope=$(function(){return T.querySelectorAll(\":scope\")}),le.cssHas=$(function(){try{return T.querySelector(\":has(*,:jqfake)\"),!1}catch(e){return!0}}),le.getById?(b.filter.ID=function(e){var t=e.replace(O,P);return function(e){return e.getAttribute(\"id\")===t}},b.find.ID=function(e,t){if(\"undefined\"!=typeof t.getElementById&&C){var n=t.getElementById(e);return n?[n]:[]}}):(b.filter.ID=function(e){var n=e.replace(O,P);return function(e){var t=\"undefined\"!=typeof e.getAttributeNode&&e.getAttributeNode(\"id\");return t&&t.value===n}},b.find.ID=function(e,t){if(\"undefined\"!=typeof t.getElementById&&C){var n,r,i,o=t.getElementById(e);if(o){if((n=o.getAttributeNode(\"id\"))&&n.value===e)return[o];i=t.getElementsByName(e),r=0;while(o=i[r++])if((n=o.getAttributeNode(\"id\"))&&n.value===e)return[o]}return[]}}),b.find.TAG=function(e,t){return\"undefined\"!=typeof t.getElementsByTagName?t.getElementsByTagName(e):t.querySelectorAll(e)},b.find.CLASS=function(e,t){if(\"undefined\"!=typeof t.getElementsByClassName&&C)return t.getElementsByClassName(e)},d=[],$(function(e){var t;r.appendChild(e).innerHTML=\"<a id='\"+S+\"' href='' disabled='disabled'></a><select id='\"+S+\"-\\r\\\\' disabled='disabled'><option selected=''></option></select>\",e.querySelectorAll(\"[selected]\").length||d.push(\"\\\\[\"+ge+\"*(?:value|\"+f+\")\"),e.querySelectorAll(\"[id~=\"+S+\"-]\").length||d.push(\"~=\"),e.querySelectorAll(\"a#\"+S+\"+*\").length||d.push(\".#.+[+~]\"),e.querySelectorAll(\":checked\").length||d.push(\":checked\"),(t=T.createElement(\"input\")).setAttribute(\"type\",\"hidden\"),e.appendChild(t).setAttribute(\"name\",\"D\"),r.appendChild(e).disabled=!0,2!==e.querySelectorAll(\":disabled\").length&&d.push(\":enabled\",\":disabled\"),(t=T.createElement(\"input\")).setAttribute(\"name\",\"\"),e.appendChild(t),e.querySelectorAll(\"[name='']\").length||d.push(\"\\\\[\"+ge+\"*name\"+ge+\"*=\"+ge+\"*(?:''|\\\"\\\")\")}),le.cssHas||d.push(\":has\"),d=d.length&&new RegExp(d.join(\"|\")),l=function(e,t){if(e===t)return a=!0,0;var n=!e.compareDocumentPosition-!t.compareDocumentPosition;return n||(1&(n=(e.ownerDocument||e)==(t.ownerDocument||t)?e.compareDocumentPosition(t):1)||!le.sortDetached&&t.compareDocumentPosition(e)===n?e===T||e.ownerDocument==ye&&I.contains(ye,e)?-1:t===T||t.ownerDocument==ye&&I.contains(ye,t)?1:o?se.call(o,e)-se.call(o,t):0:4&n?-1:1)}),T}for(e in I.matches=function(e,t){return I(e,null,null,t)},I.matchesSelector=function(e,t){if(V(e),C&&!h[t+\" \"]&&(!d||!d.test(t)))try{var n=i.call(e,t);if(n||le.disconnectedMatch||e.document&&11!==e.document.nodeType)return n}catch(e){h(t,!0)}return 0<I(t,T,null,[e]).length},I.contains=function(e,t){return(e.ownerDocument||e)!=T&&V(e),ce.contains(e,t)},I.attr=function(e,t){(e.ownerDocument||e)!=T&&V(e);var n=b.attrHandle[t.toLowerCase()],r=n&&ue.call(b.attrHandle,t.toLowerCase())?n(e,t,!C):void 0;return void 0!==r?r:e.getAttribute(t)},I.error=function(e){throw new Error(\"Syntax error, unrecognized expression: \"+e)},ce.uniqueSort=function(e){var t,n=[],r=0,i=0;if(a=!le.sortStable,o=!le.sortStable&&ae.call(e,0),de.call(e,l),a){while(t=e[i++])t===e[i]&&(r=n.push(i));while(r--)he.call(e,n[r],1)}return o=null,e},ce.fn.uniqueSort=function(){return this.pushStack(ce.uniqueSort(ae.apply(this)))},(b=ce.expr={cacheLength:50,createPseudo:F,match:D,attrHandle:{},find:{},relative:{\">\":{dir:\"parentNode\",first:!0},\" \":{dir:\"parentNode\"},\"+\":{dir:\"previousSibling\",first:!0},\"~\":{dir:\"previousSibling\"}},preFilter:{ATTR:function(e){return e[1]=e[1].replace(O,P),e[3]=(e[3]||e[4]||e[5]||\"\").replace(O,P),\"~=\"===e[2]&&(e[3]=\" \"+e[3]+\" \"),e.slice(0,4)},CHILD:function(e){return e[1]=e[1].toLowerCase(),\"nth\"===e[1].slice(0,3)?(e[3]||I.error(e[0]),e[4]=+(e[4]?e[5]+(e[6]||1):2*(\"even\"===e[3]||\"odd\"===e[3])),e[5]=+(e[7]+e[8]||\"odd\"===e[3])):e[3]&&I.error(e[0]),e},PSEUDO:function(e){var t,n=!e[6]&&e[2];return D.CHILD.test(e[0])?null:(e[3]?e[2]=e[4]||e[5]||\"\":n&&j.test(n)&&(t=Y(n,!0))&&(t=n.indexOf(\")\",n.length-t)-n.length)&&(e[0]=e[0].slice(0,t),e[2]=n.slice(0,t)),e.slice(0,3))}},filter:{TAG:function(e){var t=e.replace(O,P).toLowerCase();return\"*\"===e?function(){return!0}:function(e){return fe(e,t)}},CLASS:function(e){var t=s[e+\" \"];return t||(t=new RegExp(\"(^|\"+ge+\")\"+e+\"(\"+ge+\"|$)\"))&&s(e,function(e){return t.test(\"string\"==typeof e.className&&e.className||\"undefined\"!=typeof e.getAttribute&&e.getAttribute(\"class\")||\"\")})},ATTR:function(n,r,i){return function(e){var t=I.attr(e,n);return null==t?\"!=\"===r:!r||(t+=\"\",\"=\"===r?t===i:\"!=\"===r?t!==i:\"^=\"===r?i&&0===t.indexOf(i):\"*=\"===r?i&&-1<t.indexOf(i):\"$=\"===r?i&&t.slice(-i.length)===i:\"~=\"===r?-1<(\" \"+t.replace(v,\" \")+\" \").indexOf(i):\"|=\"===r&&(t===i||t.slice(0,i.length+1)===i+\"-\"))}},CHILD:function(d,e,t,h,g){var v=\"nth\"!==d.slice(0,3),y=\"last\"!==d.slice(-4),m=\"of-type\"===e;return 1===h&&0===g?function(e){return!!e.parentNode}:function(e,t,n){var r,i,o,a,s,u=v!==y?\"nextSibling\":\"previousSibling\",l=e.parentNode,c=m&&e.nodeName.toLowerCase(),f=!n&&!m,p=!1;if(l){if(v){while(u){o=e;while(o=o[u])if(m?fe(o,c):1===o.nodeType)return!1;s=u=\"only\"===d&&!s&&\"nextSibling\"}return!0}if(s=[y?l.firstChild:l.lastChild],y&&f){p=(a=(r=(i=l[S]||(l[S]={}))[d]||[])[0]===E&&r[1])&&r[2],o=a&&l.childNodes[a];while(o=++a&&o&&o[u]||(p=a=0)||s.pop())if(1===o.nodeType&&++p&&o===e){i[d]=[E,a,p];break}}else if(f&&(p=a=(r=(i=e[S]||(e[S]={}))[d]||[])[0]===E&&r[1]),!1===p)while(o=++a&&o&&o[u]||(p=a=0)||s.pop())if((m?fe(o,c):1===o.nodeType)&&++p&&(f&&((i=o[S]||(o[S]={}))[d]=[E,p]),o===e))break;return(p-=g)===h||p%h==0&&0<=p/h}}},PSEUDO:function(e,o){var t,a=b.pseudos[e]||b.setFilters[e.toLowerCase()]||I.error(\"unsupported pseudo: \"+e);return a[S]?a(o):1<a.length?(t=[e,e,\"\",o],b.setFilters.hasOwnProperty(e.toLowerCase())?F(function(e,t){var n,r=a(e,o),i=r.length;while(i--)e[n=se.call(e,r[i])]=!(t[n]=r[i])}):function(e){return a(e,0,t)}):a}},pseudos:{not:F(function(e){var r=[],i=[],s=ne(e.replace(ve,\"$1\"));return s[S]?F(function(e,t,n,r){var i,o=s(e,null,r,[]),a=e.length;while(a--)(i=o[a])&&(e[a]=!(t[a]=i))}):function(e,t,n){return r[0]=e,s(r,null,n,i),r[0]=null,!i.pop()}}),has:F(function(t){return function(e){return 0<I(t,e).length}}),contains:F(function(t){return t=t.replace(O,P),function(e){return-1<(e.textContent||ce.text(e)).indexOf(t)}}),lang:F(function(n){return A.test(n||\"\")||I.error(\"unsupported lang: \"+n),n=n.replace(O,P).toLowerCase(),function(e){var t;do{if(t=C?e.lang:e.getAttribute(\"xml:lang\")||e.getAttribute(\"lang\"))return(t=t.toLowerCase())===n||0===t.indexOf(n+\"-\")}while((e=e.parentNode)&&1===e.nodeType);return!1}}),target:function(e){var t=ie.location&&ie.location.hash;return t&&t.slice(1)===e.id},root:function(e){return e===r},focus:function(e){return e===function(){try{return T.activeElement}catch(e){}}()&&T.hasFocus()&&!!(e.type||e.href||~e.tabIndex)},enabled:z(!1),disabled:z(!0),checked:function(e){return fe(e,\"input\")&&!!e.checked||fe(e,\"option\")&&!!e.selected},selected:function(e){return e.parentNode&&e.parentNode.selectedIndex,!0===e.selected},empty:function(e){for(e=e.firstChild;e;e=e.nextSibling)if(e.nodeType<6)return!1;return!0},parent:function(e){return!b.pseudos.empty(e)},header:function(e){return q.test(e.nodeName)},input:function(e){return N.test(e.nodeName)},button:function(e){return fe(e,\"input\")&&\"button\"===e.type||fe(e,\"button\")},text:function(e){var t;return fe(e,\"input\")&&\"text\"===e.type&&(null==(t=e.getAttribute(\"type\"))||\"text\"===t.toLowerCase())},first:X(function(){return[0]}),last:X(function(e,t){return[t-1]}),eq:X(function(e,t,n){return[n<0?n+t:n]}),even:X(function(e,t){for(var n=0;n<t;n+=2)e.push(n);return e}),odd:X(function(e,t){for(var n=1;n<t;n+=2)e.push(n);return e}),lt:X(function(e,t,n){var r;for(r=n<0?n+t:t<n?t:n;0<=--r;)e.push(r);return e}),gt:X(function(e,t,n){for(var r=n<0?n+t:n;++r<t;)e.push(r);return e})}}).pseudos.nth=b.pseudos.eq,{radio:!0,checkbox:!0,file:!0,password:!0,image:!0})b.pseudos[e]=B(e);for(e in{submit:!0,reset:!0})b.pseudos[e]=_(e);function G(){}function Y(e,t){var n,r,i,o,a,s,u,l=c[e+\" \"];if(l)return t?0:l.slice(0);a=e,s=[],u=b.preFilter;while(a){for(o in n&&!(r=y.exec(a))||(r&&(a=a.slice(r[0].length)||a),s.push(i=[])),n=!1,(r=m.exec(a))&&(n=r.shift(),i.push({value:n,type:r[0].replace(ve,\" \")}),a=a.slice(n.length)),b.filter)!(r=D[o].exec(a))||u[o]&&!(r=u[o](r))||(n=r.shift(),i.push({value:n,type:o,matches:r}),a=a.slice(n.length));if(!n)break}return t?a.length:a?I.error(e):c(e,s).slice(0)}function Q(e){for(var t=0,n=e.length,r=\"\";t<n;t++)r+=e[t].value;return r}function J(a,e,t){var s=e.dir,u=e.next,l=u||s,c=t&&\"parentNode\"===l,f=n++;return e.first?function(e,t,n){while(e=e[s])if(1===e.nodeType||c)return a(e,t,n);return!1}:function(e,t,n){var r,i,o=[E,f];if(n){while(e=e[s])if((1===e.nodeType||c)&&a(e,t,n))return!0}else while(e=e[s])if(1===e.nodeType||c)if(i=e[S]||(e[S]={}),u&&fe(e,u))e=e[s]||e;else{if((r=i[l])&&r[0]===E&&r[1]===f)return o[2]=r[2];if((i[l]=o)[2]=a(e,t,n))return!0}return!1}}function K(i){return 1<i.length?function(e,t,n){var r=i.length;while(r--)if(!i[r](e,t,n))return!1;return!0}:i[0]}function Z(e,t,n,r,i){for(var o,a=[],s=0,u=e.length,l=null!=t;s<u;s++)(o=e[s])&&(n&&!n(o,r,i)||(a.push(o),l&&t.push(s)));return a}function ee(d,h,g,v,y,e){return v&&!v[S]&&(v=ee(v)),y&&!y[S]&&(y=ee(y,e)),F(function(e,t,n,r){var i,o,a,s,u=[],l=[],c=t.length,f=e||function(e,t,n){for(var r=0,i=t.length;r<i;r++)I(e,t[r],n);return n}(h||\"*\",n.nodeType?[n]:n,[]),p=!d||!e&&h?f:Z(f,u,d,n,r);if(g?g(p,s=y||(e?d:c||v)?[]:t,n,r):s=p,v){i=Z(s,l),v(i,[],n,r),o=i.length;while(o--)(a=i[o])&&(s[l[o]]=!(p[l[o]]=a))}if(e){if(y||d){if(y){i=[],o=s.length;while(o--)(a=s[o])&&i.push(p[o]=a);y(null,s=[],i,r)}o=s.length;while(o--)(a=s[o])&&-1<(i=y?se.call(e,a):u[o])&&(e[i]=!(t[i]=a))}}else s=Z(s===t?s.splice(c,s.length):s),y?y(null,t,s,r):k.apply(t,s)})}function te(e){for(var i,t,n,r=e.length,o=b.relative[e[0].type],a=o||b.relative[\" \"],s=o?1:0,u=J(function(e){return e===i},a,!0),l=J(function(e){return-1<se.call(i,e)},a,!0),c=[function(e,t,n){var r=!o&&(n||t!=w)||((i=t).nodeType?u(e,t,n):l(e,t,n));return i=null,r}];s<r;s++)if(t=b.relative[e[s].type])c=[J(K(c),t)];else{if((t=b.filter[e[s].type].apply(null,e[s].matches))[S]){for(n=++s;n<r;n++)if(b.relative[e[n].type])break;return ee(1<s&&K(c),1<s&&Q(e.slice(0,s-1).concat({value:\" \"===e[s-2].type?\"*\":\"\"})).replace(ve,\"$1\"),t,s<n&&te(e.slice(s,n)),n<r&&te(e=e.slice(n)),n<r&&Q(e))}c.push(t)}return K(c)}function ne(e,t){var n,v,y,m,x,r,i=[],o=[],a=u[e+\" \"];if(!a){t||(t=Y(e)),n=t.length;while(n--)(a=te(t[n]))[S]?i.push(a):o.push(a);(a=u(e,(v=o,m=0<(y=i).length,x=0<v.length,r=function(e,t,n,r,i){var o,a,s,u=0,l=\"0\",c=e&&[],f=[],p=w,d=e||x&&b.find.TAG(\"*\",i),h=E+=null==p?1:Math.random()||.1,g=d.length;for(i&&(w=t==T||t||i);l!==g&&null!=(o=d[l]);l++){if(x&&o){a=0,t||o.ownerDocument==T||(V(o),n=!C);while(s=v[a++])if(s(o,t||T,n)){k.call(r,o);break}i&&(E=h)}m&&((o=!s&&o)&&u--,e&&c.push(o))}if(u+=l,m&&l!==u){a=0;while(s=y[a++])s(c,f,t,n);if(e){if(0<u)while(l--)c[l]||f[l]||(f[l]=pe.call(r));f=Z(f)}k.apply(r,f),i&&!e&&0<f.length&&1<u+y.length&&ce.uniqueSort(r)}return i&&(E=h,w=p),c},m?F(r):r))).selector=e}return a}function re(e,t,n,r){var i,o,a,s,u,l=\"function\"==typeof e&&e,c=!r&&Y(e=l.selector||e);if(n=n||[],1===c.length){if(2<(o=c[0]=c[0].slice(0)).length&&\"ID\"===(a=o[0]).type&&9===t.nodeType&&C&&b.relative[o[1].type]){if(!(t=(b.find.ID(a.matches[0].replace(O,P),t)||[])[0]))return n;l&&(t=t.parentNode),e=e.slice(o.shift().value.length)}i=D.needsContext.test(e)?0:o.length;while(i--){if(a=o[i],b.relative[s=a.type])break;if((u=b.find[s])&&(r=u(a.matches[0].replace(O,P),H.test(o[0].type)&&U(t.parentNode)||t))){if(o.splice(i,1),!(e=r.length&&Q(o)))return k.apply(n,r),n;break}}}return(l||ne(e,c))(r,t,!C,n,!t||H.test(e)&&U(t.parentNode)||t),n}G.prototype=b.filters=b.pseudos,b.setFilters=new G,le.sortStable=S.split(\"\").sort(l).join(\"\")===S,V(),le.sortDetached=$(function(e){return 1&e.compareDocumentPosition(T.createElement(\"fieldset\"))}),ce.find=I,ce.expr[\":\"]=ce.expr.pseudos,ce.unique=ce.uniqueSort,I.compile=ne,I.select=re,I.setDocument=V,I.tokenize=Y,I.escape=ce.escapeSelector,I.getText=ce.text,I.isXML=ce.isXMLDoc,I.selectors=ce.expr,I.support=ce.support,I.uniqueSort=ce.uniqueSort}();var d=function(e,t,n){var r=[],i=void 0!==n;while((e=e[t])&&9!==e.nodeType)if(1===e.nodeType){if(i&&ce(e).is(n))break;r.push(e)}return r},h=function(e,t){for(var n=[];e;e=e.nextSibling)1===e.nodeType&&e!==t&&n.push(e);return n},b=ce.expr.match.needsContext,w=/^<([a-z][^\\/\\0>:\\x20\\t\\r\\n\\f]*)[\\x20\\t\\r\\n\\f]*\\/?>(?:<\\/\\1>|)$/i;function T(e,n,r){return v(n)?ce.grep(e,function(e,t){return!!n.call(e,t,e)!==r}):n.nodeType?ce.grep(e,function(e){return e===n!==r}):\"string\"!=typeof n?ce.grep(e,function(e){return-1<se.call(n,e)!==r}):ce.filter(n,e,r)}ce.filter=function(e,t,n){var r=t[0];return n&&(e=\":not(\"+e+\")\"),1===t.length&&1===r.nodeType?ce.find.matchesSelector(r,e)?[r]:[]:ce.find.matches(e,ce.grep(t,function(e){return 1===e.nodeType}))},ce.fn.extend({find:function(e){var t,n,r=this.length,i=this;if(\"string\"!=typeof e)return this.pushStack(ce(e).filter(function(){for(t=0;t<r;t++)if(ce.contains(i[t],this))return!0}));for(n=this.pushStack([]),t=0;t<r;t++)ce.find(e,i[t],n);return 1<r?ce.uniqueSort(n):n},filter:function(e){return this.pushStack(T(this,e||[],!1))},not:function(e){return this.pushStack(T(this,e||[],!0))},is:function(e){return!!T(this,\"string\"==typeof e&&b.test(e)?ce(e):e||[],!1).length}});var k,S=/^(?:\\s*(<[\\w\\W]+>)[^>]*|#([\\w-]+))$/;(ce.fn.init=function(e,t,n){var r,i;if(!e)return this;if(n=n||k,\"string\"==typeof e){if(!(r=\"<\"===e[0]&&\">\"===e[e.length-1]&&3<=e.length?[null,e,null]:S.exec(e))||!r[1]&&t)return!t||t.jquery?(t||n).find(e):this.constructor(t).find(e);if(r[1]){if(t=t instanceof ce?t[0]:t,ce.merge(this,ce.parseHTML(r[1],t&&t.nodeType?t.ownerDocument||t:C,!0)),w.test(r[1])&&ce.isPlainObject(t))for(r in t)v(this[r])?this[r](t[r]):this.attr(r,t[r]);return this}return(i=C.getElementById(r[2]))&&(this[0]=i,this.length=1),this}return e.nodeType?(this[0]=e,this.length=1,this):v(e)?void 0!==n.ready?n.ready(e):e(ce):ce.makeArray(e,this)}).prototype=ce.fn,k=ce(C);var E=/^(?:parents|prev(?:Until|All))/,j={children:!0,contents:!0,next:!0,prev:!0};function A(e,t){while((e=e[t])&&1!==e.nodeType);return e}ce.fn.extend({has:function(e){var t=ce(e,this),n=t.length;return this.filter(function(){for(var e=0;e<n;e++)if(ce.contains(this,t[e]))return!0})},closest:function(e,t){var n,r=0,i=this.length,o=[],a=\"string\"!=typeof e&&ce(e);if(!b.test(e))for(;r<i;r++)for(n=this[r];n&&n!==t;n=n.parentNode)if(n.nodeType<11&&(a?-1<a.index(n):1===n.nodeType&&ce.find.matchesSelector(n,e))){o.push(n);break}return this.pushStack(1<o.length?ce.uniqueSort(o):o)},index:function(e){return e?\"string\"==typeof e?se.call(ce(e),this[0]):se.call(this,e.jquery?e[0]:e):this[0]&&this[0].parentNode?this.first().prevAll().length:-1},add:function(e,t){return this.pushStack(ce.uniqueSort(ce.merge(this.get(),ce(e,t))))},addBack:function(e){return this.add(null==e?this.prevObject:this.prevObject.filter(e))}}),ce.each({parent:function(e){var t=e.parentNode;return t&&11!==t.nodeType?t:null},parents:function(e){return d(e,\"parentNode\")},parentsUntil:function(e,t,n){return d(e,\"parentNode\",n)},next:function(e){return A(e,\"nextSibling\")},prev:function(e){return A(e,\"previousSibling\")},nextAll:function(e){return d(e,\"nextSibling\")},prevAll:function(e){return d(e,\"previousSibling\")},nextUntil:function(e,t,n){return d(e,\"nextSibling\",n)},prevUntil:function(e,t,n){return d(e,\"previousSibling\",n)},siblings:function(e){return h((e.parentNode||{}).firstChild,e)},children:function(e){return h(e.firstChild)},contents:function(e){return null!=e.contentDocument&&r(e.contentDocument)?e.contentDocument:(fe(e,\"template\")&&(e=e.content||e),ce.merge([],e.childNodes))}},function(r,i){ce.fn[r]=function(e,t){var n=ce.map(this,i,e);return\"Until\"!==r.slice(-5)&&(t=e),t&&\"string\"==typeof t&&(n=ce.filter(t,n)),1<this.length&&(j[r]||ce.uniqueSort(n),E.test(r)&&n.reverse()),this.pushStack(n)}});var D=/[^\\x20\\t\\r\\n\\f]+/g;function N(e){return e}function q(e){throw e}function L(e,t,n,r){var i;try{e&&v(i=e.promise)?i.call(e).done(t).fail(n):e&&v(i=e.then)?i.call(e,t,n):t.apply(void 0,[e].slice(r))}catch(e){n.apply(void 0,[e])}}ce.Callbacks=function(r){var e,n;r=\"string\"==typeof r?(e=r,n={},ce.each(e.match(D)||[],function(e,t){n[t]=!0}),n):ce.extend({},r);var i,t,o,a,s=[],u=[],l=-1,c=function(){for(a=a||r.once,o=i=!0;u.length;l=-1){t=u.shift();while(++l<s.length)!1===s[l].apply(t[0],t[1])&&r.stopOnFalse&&(l=s.length,t=!1)}r.memory||(t=!1),i=!1,a&&(s=t?[]:\"\")},f={add:function(){return s&&(t&&!i&&(l=s.length-1,u.push(t)),function n(e){ce.each(e,function(e,t){v(t)?r.unique&&f.has(t)||s.push(t):t&&t.length&&\"string\"!==x(t)&&n(t)})}(arguments),t&&!i&&c()),this},remove:function(){return ce.each(arguments,function(e,t){var n;while(-1<(n=ce.inArray(t,s,n)))s.splice(n,1),n<=l&&l--}),this},has:function(e){return e?-1<ce.inArray(e,s):0<s.length},empty:function(){return s&&(s=[]),this},disable:function(){return a=u=[],s=t=\"\",this},disabled:function(){return!s},lock:function(){return a=u=[],t||i||(s=t=\"\"),this},locked:function(){return!!a},fireWith:function(e,t){return a||(t=[e,(t=t||[]).slice?t.slice():t],u.push(t),i||c()),this},fire:function(){return f.fireWith(this,arguments),this},fired:function(){return!!o}};return f},ce.extend({Deferred:function(e){var o=[[\"notify\",\"progress\",ce.Callbacks(\"memory\"),ce.Callbacks(\"memory\"),2],[\"resolve\",\"done\",ce.Callbacks(\"once memory\"),ce.Callbacks(\"once memory\"),0,\"resolved\"],[\"reject\",\"fail\",ce.Callbacks(\"once memory\"),ce.Callbacks(\"once memory\"),1,\"rejected\"]],i=\"pending\",a={state:function(){return i},always:function(){return s.done(arguments).fail(arguments),this},\"catch\":function(e){return a.then(null,e)},pipe:function(){var i=arguments;return ce.Deferred(function(r){ce.each(o,function(e,t){var n=v(i[t[4]])&&i[t[4]];s[t[1]](function(){var e=n&&n.apply(this,arguments);e&&v(e.promise)?e.promise().progress(r.notify).done(r.resolve).fail(r.reject):r[t[0]+\"With\"](this,n?[e]:arguments)})}),i=null}).promise()},then:function(t,n,r){var u=0;function l(i,o,a,s){return function(){var n=this,r=arguments,e=function(){var e,t;if(!(i<u)){if((e=a.apply(n,r))===o.promise())throw new TypeError(\"Thenable self-resolution\");t=e&&(\"object\"==typeof e||\"function\"==typeof e)&&e.then,v(t)?s?t.call(e,l(u,o,N,s),l(u,o,q,s)):(u++,t.call(e,l(u,o,N,s),l(u,o,q,s),l(u,o,N,o.notifyWith))):(a!==N&&(n=void 0,r=[e]),(s||o.resolveWith)(n,r))}},t=s?e:function(){try{e()}catch(e){ce.Deferred.exceptionHook&&ce.Deferred.exceptionHook(e,t.error),u<=i+1&&(a!==q&&(n=void 0,r=[e]),o.rejectWith(n,r))}};i?t():(ce.Deferred.getErrorHook?t.error=ce.Deferred.getErrorHook():ce.Deferred.getStackHook&&(t.error=ce.Deferred.getStackHook()),ie.setTimeout(t))}}return ce.Deferred(function(e){o[0][3].add(l(0,e,v(r)?r:N,e.notifyWith)),o[1][3].add(l(0,e,v(t)?t:N)),o[2][3].add(l(0,e,v(n)?n:q))}).promise()},promise:function(e){return null!=e?ce.extend(e,a):a}},s={};return ce.each(o,function(e,t){var n=t[2],r=t[5];a[t[1]]=n.add,r&&n.add(function(){i=r},o[3-e][2].disable,o[3-e][3].disable,o[0][2].lock,o[0][3].lock),n.add(t[3].fire),s[t[0]]=function(){return s[t[0]+\"With\"](this===s?void 0:this,arguments),this},s[t[0]+\"With\"]=n.fireWith}),a.promise(s),e&&e.call(s,s),s},when:function(e){var n=arguments.length,t=n,r=Array(t),i=ae.call(arguments),o=ce.Deferred(),a=function(t){return function(e){r[t]=this,i[t]=1<arguments.length?ae.call(arguments):e,--n||o.resolveWith(r,i)}};if(n<=1&&(L(e,o.done(a(t)).resolve,o.reject,!n),\"pending\"===o.state()||v(i[t]&&i[t].then)))return o.then();while(t--)L(i[t],a(t),o.reject);return o.promise()}});var H=/^(Eval|Internal|Range|Reference|Syntax|Type|URI)Error$/;ce.Deferred.exceptionHook=function(e,t){ie.console&&ie.console.warn&&e&&H.test(e.name)&&ie.console.warn(\"jQuery.Deferred exception: \"+e.message,e.stack,t)},ce.readyException=function(e){ie.setTimeout(function(){throw e})};var O=ce.Deferred();function P(){C.removeEventListener(\"DOMContentLoaded\",P),ie.removeEventListener(\"load\",P),ce.ready()}ce.fn.ready=function(e){return O.then(e)[\"catch\"](function(e){ce.readyException(e)}),this},ce.extend({isReady:!1,readyWait:1,ready:function(e){(!0===e?--ce.readyWait:ce.isReady)||(ce.isReady=!0)!==e&&0<--ce.readyWait||O.resolveWith(C,[ce])}}),ce.ready.then=O.then,\"complete\"===C.readyState||\"loading\"!==C.readyState&&!C.documentElement.doScroll?ie.setTimeout(ce.ready):(C.addEventListener(\"DOMContentLoaded\",P),ie.addEventListener(\"load\",P));var M=function(e,t,n,r,i,o,a){var s=0,u=e.length,l=null==n;if(\"object\"===x(n))for(s in i=!0,n)M(e,t,s,n[s],!0,o,a);else if(void 0!==r&&(i=!0,v(r)||(a=!0),l&&(a?(t.call(e,r),t=null):(l=t,t=function(e,t,n){return l.call(ce(e),n)})),t))for(;s<u;s++)t(e[s],n,a?r:r.call(e[s],s,t(e[s],n)));return i?e:l?t.call(e):u?t(e[0],n):o},R=/^-ms-/,I=/-([a-z])/g;function W(e,t){return t.toUpperCase()}function F(e){return e.replace(R,\"ms-\").replace(I,W)}var $=function(e){return 1===e.nodeType||9===e.nodeType||!+e.nodeType};function B(){this.expando=ce.expando+B.uid++}B.uid=1,B.prototype={cache:function(e){var t=e[this.expando];return t||(t={},$(e)&&(e.nodeType?e[this.expando]=t:Object.defineProperty(e,this.expando,{value:t,configurable:!0}))),t},set:function(e,t,n){var r,i=this.cache(e);if(\"string\"==typeof t)i[F(t)]=n;else for(r in t)i[F(r)]=t[r];return i},get:function(e,t){return void 0===t?this.cache(e):e[this.expando]&&e[this.expando][F(t)]},access:function(e,t,n){return void 0===t||t&&\"string\"==typeof t&&void 0===n?this.get(e,t):(this.set(e,t,n),void 0!==n?n:t)},remove:function(e,t){var n,r=e[this.expando];if(void 0!==r){if(void 0!==t){n=(t=Array.isArray(t)?t.map(F):(t=F(t))in r?[t]:t.match(D)||[]).length;while(n--)delete r[t[n]]}(void 0===t||ce.isEmptyObject(r))&&(e.nodeType?e[this.expando]=void 0:delete e[this.expando])}},hasData:function(e){var t=e[this.expando];return void 0!==t&&!ce.isEmptyObject(t)}};var _=new B,z=new B,X=/^(?:\\{[\\w\\W]*\\}|\\[[\\w\\W]*\\])$/,U=/[A-Z]/g;function V(e,t,n){var r,i;if(void 0===n&&1===e.nodeType)if(r=\"data-\"+t.replace(U,\"-$&\").toLowerCase(),\"string\"==typeof(n=e.getAttribute(r))){try{n=\"true\"===(i=n)||\"false\"!==i&&(\"null\"===i?null:i===+i+\"\"?+i:X.test(i)?JSON.parse(i):i)}catch(e){}z.set(e,t,n)}else n=void 0;return n}ce.extend({hasData:function(e){return z.hasData(e)||_.hasData(e)},data:function(e,t,n){return z.access(e,t,n)},removeData:function(e,t){z.remove(e,t)},_data:function(e,t,n){return _.access(e,t,n)},_removeData:function(e,t){_.remove(e,t)}}),ce.fn.extend({data:function(n,e){var t,r,i,o=this[0],a=o&&o.attributes;if(void 0===n){if(this.length&&(i=z.get(o),1===o.nodeType&&!_.get(o,\"hasDataAttrs\"))){t=a.length;while(t--)a[t]&&0===(r=a[t].name).indexOf(\"data-\")&&(r=F(r.slice(5)),V(o,r,i[r]));_.set(o,\"hasDataAttrs\",!0)}return i}return\"object\"==typeof n?this.each(function(){z.set(this,n)}):M(this,function(e){var t;if(o&&void 0===e)return void 0!==(t=z.get(o,n))?t:void 0!==(t=V(o,n))?t:void 0;this.each(function(){z.set(this,n,e)})},null,e,1<arguments.length,null,!0)},removeData:function(e){return this.each(function(){z.remove(this,e)})}}),ce.extend({queue:function(e,t,n){var r;if(e)return t=(t||\"fx\")+\"queue\",r=_.get(e,t),n&&(!r||Array.isArray(n)?r=_.access(e,t,ce.makeArray(n)):r.push(n)),r||[]},dequeue:function(e,t){t=t||\"fx\";var n=ce.queue(e,t),r=n.length,i=n.shift(),o=ce._queueHooks(e,t);\"inprogress\"===i&&(i=n.shift(),r--),i&&(\"fx\"===t&&n.unshift(\"inprogress\"),delete o.stop,i.call(e,function(){ce.dequeue(e,t)},o)),!r&&o&&o.empty.fire()},_queueHooks:function(e,t){var n=t+\"queueHooks\";return _.get(e,n)||_.access(e,n,{empty:ce.Callbacks(\"once memory\").add(function(){_.remove(e,[t+\"queue\",n])})})}}),ce.fn.extend({queue:function(t,n){var e=2;return\"string\"!=typeof t&&(n=t,t=\"fx\",e--),arguments.length<e?ce.queue(this[0],t):void 0===n?this:this.each(function(){var e=ce.queue(this,t,n);ce._queueHooks(this,t),\"fx\"===t&&\"inprogress\"!==e[0]&&ce.dequeue(this,t)})},dequeue:function(e){return this.each(function(){ce.dequeue(this,e)})},clearQueue:function(e){return this.queue(e||\"fx\",[])},promise:function(e,t){var n,r=1,i=ce.Deferred(),o=this,a=this.length,s=function(){--r||i.resolveWith(o,[o])};\"string\"!=typeof e&&(t=e,e=void 0),e=e||\"fx\";while(a--)(n=_.get(o[a],e+\"queueHooks\"))&&n.empty&&(r++,n.empty.add(s));return s(),i.promise(t)}});var G=/[+-]?(?:\\d*\\.|)\\d+(?:[eE][+-]?\\d+|)/.source,Y=new RegExp(\"^(?:([+-])=|)(\"+G+\")([a-z%]*)$\",\"i\"),Q=[\"Top\",\"Right\",\"Bottom\",\"Left\"],J=C.documentElement,K=function(e){return ce.contains(e.ownerDocument,e)},Z={composed:!0};J.getRootNode&&(K=function(e){return ce.contains(e.ownerDocument,e)||e.getRootNode(Z)===e.ownerDocument});var ee=function(e,t){return\"none\"===(e=t||e).style.display||\"\"===e.style.display&&K(e)&&\"none\"===ce.css(e,\"display\")};function te(e,t,n,r){var i,o,a=20,s=r?function(){return r.cur()}:function(){return ce.css(e,t,\"\")},u=s(),l=n&&n[3]||(ce.cssNumber[t]?\"\":\"px\"),c=e.nodeType&&(ce.cssNumber[t]||\"px\"!==l&&+u)&&Y.exec(ce.css(e,t));if(c&&c[3]!==l){u/=2,l=l||c[3],c=+u||1;while(a--)ce.style(e,t,c+l),(1-o)*(1-(o=s()/u||.5))<=0&&(a=0),c/=o;c*=2,ce.style(e,t,c+l),n=n||[]}return n&&(c=+c||+u||0,i=n[1]?c+(n[1]+1)*n[2]:+n[2],r&&(r.unit=l,r.start=c,r.end=i)),i}var ne={};function re(e,t){for(var n,r,i,o,a,s,u,l=[],c=0,f=e.length;c<f;c++)(r=e[c]).style&&(n=r.style.display,t?(\"none\"===n&&(l[c]=_.get(r,\"display\")||null,l[c]||(r.style.display=\"\")),\"\"===r.style.display&&ee(r)&&(l[c]=(u=a=o=void 0,a=(i=r).ownerDocument,s=i.nodeName,(u=ne[s])||(o=a.body.appendChild(a.createElement(s)),u=ce.css(o,\"display\"),o.parentNode.removeChild(o),\"none\"===u&&(u=\"block\"),ne[s]=u)))):\"none\"!==n&&(l[c]=\"none\",_.set(r,\"display\",n)));for(c=0;c<f;c++)null!=l[c]&&(e[c].style.display=l[c]);return e}ce.fn.extend({show:function(){return re(this,!0)},hide:function(){return re(this)},toggle:function(e){return\"boolean\"==typeof e?e?this.show():this.hide():this.each(function(){ee(this)?ce(this).show():ce(this).hide()})}});var xe,be,we=/^(?:checkbox|radio)$/i,Te=/<([a-z][^\\/\\0>\\x20\\t\\r\\n\\f]*)/i,Ce=/^$|^module$|\\/(?:java|ecma)script/i;xe=C.createDocumentFragment().appendChild(C.createElement(\"div\")),(be=C.createElement(\"input\")).setAttribute(\"type\",\"radio\"),be.setAttribute(\"checked\",\"checked\"),be.setAttribute(\"name\",\"t\"),xe.appendChild(be),le.checkClone=xe.cloneNode(!0).cloneNode(!0).lastChild.checked,xe.innerHTML=\"<textarea>x</textarea>\",le.noCloneChecked=!!xe.cloneNode(!0).lastChild.defaultValue,xe.innerHTML=\"<option></option>\",le.option=!!xe.lastChild;var ke={thead:[1,\"<table>\",\"</table>\"],col:[2,\"<table><colgroup>\",\"</colgroup></table>\"],tr:[2,\"<table><tbody>\",\"</tbody></table>\"],td:[3,\"<table><tbody><tr>\",\"</tr></tbody></table>\"],_default:[0,\"\",\"\"]};function Se(e,t){var n;return n=\"undefined\"!=typeof e.getElementsByTagName?e.getElementsByTagName(t||\"*\"):\"undefined\"!=typeof e.querySelectorAll?e.querySelectorAll(t||\"*\"):[],void 0===t||t&&fe(e,t)?ce.merge([e],n):n}function Ee(e,t){for(var n=0,r=e.length;n<r;n++)_.set(e[n],\"globalEval\",!t||_.get(t[n],\"globalEval\"))}ke.tbody=ke.tfoot=ke.colgroup=ke.caption=ke.thead,ke.th=ke.td,le.option||(ke.optgroup=ke.option=[1,\"<select multiple='multiple'>\",\"</select>\"]);var je=/<|&#?\\w+;/;function Ae(e,t,n,r,i){for(var o,a,s,u,l,c,f=t.createDocumentFragment(),p=[],d=0,h=e.length;d<h;d++)if((o=e[d])||0===o)if(\"object\"===x(o))ce.merge(p,o.nodeType?[o]:o);else if(je.test(o)){a=a||f.appendChild(t.createElement(\"div\")),s=(Te.exec(o)||[\"\",\"\"])[1].toLowerCase(),u=ke[s]||ke._default,a.innerHTML=u[1]+ce.htmlPrefilter(o)+u[2],c=u[0];while(c--)a=a.lastChild;ce.merge(p,a.childNodes),(a=f.firstChild).textContent=\"\"}else p.push(t.createTextNode(o));f.textContent=\"\",d=0;while(o=p[d++])if(r&&-1<ce.inArray(o,r))i&&i.push(o);else if(l=K(o),a=Se(f.appendChild(o),\"script\"),l&&Ee(a),n){c=0;while(o=a[c++])Ce.test(o.type||\"\")&&n.push(o)}return f}var De=/^([^.]*)(?:\\.(.+)|)/;function Ne(){return!0}function qe(){return!1}function Le(e,t,n,r,i,o){var a,s;if(\"object\"==typeof t){for(s in\"string\"!=typeof n&&(r=r||n,n=void 0),t)Le(e,s,n,r,t[s],o);return e}if(null==r&&null==i?(i=n,r=n=void 0):null==i&&(\"string\"==typeof n?(i=r,r=void 0):(i=r,r=n,n=void 0)),!1===i)i=qe;else if(!i)return e;return 1===o&&(a=i,(i=function(e){return ce().off(e),a.apply(this,arguments)}).guid=a.guid||(a.guid=ce.guid++)),e.each(function(){ce.event.add(this,t,i,r,n)})}function He(e,r,t){t?(_.set(e,r,!1),ce.event.add(e,r,{namespace:!1,handler:function(e){var t,n=_.get(this,r);if(1&e.isTrigger&&this[r]){if(n)(ce.event.special[r]||{}).delegateType&&e.stopPropagation();else if(n=ae.call(arguments),_.set(this,r,n),this[r](),t=_.get(this,r),_.set(this,r,!1),n!==t)return e.stopImmediatePropagation(),e.preventDefault(),t}else n&&(_.set(this,r,ce.event.trigger(n[0],n.slice(1),this)),e.stopPropagation(),e.isImmediatePropagationStopped=Ne)}})):void 0===_.get(e,r)&&ce.event.add(e,r,Ne)}ce.event={global:{},add:function(t,e,n,r,i){var o,a,s,u,l,c,f,p,d,h,g,v=_.get(t);if($(t)){n.handler&&(n=(o=n).handler,i=o.selector),i&&ce.find.matchesSelector(J,i),n.guid||(n.guid=ce.guid++),(u=v.events)||(u=v.events=Object.create(null)),(a=v.handle)||(a=v.handle=function(e){return\"undefined\"!=typeof ce&&ce.event.triggered!==e.type?ce.event.dispatch.apply(t,arguments):void 0}),l=(e=(e||\"\").match(D)||[\"\"]).length;while(l--)d=g=(s=De.exec(e[l])||[])[1],h=(s[2]||\"\").split(\".\").sort(),d&&(f=ce.event.special[d]||{},d=(i?f.delegateType:f.bindType)||d,f=ce.event.special[d]||{},c=ce.extend({type:d,origType:g,data:r,handler:n,guid:n.guid,selector:i,needsContext:i&&ce.expr.match.needsContext.test(i),namespace:h.join(\".\")},o),(p=u[d])||((p=u[d]=[]).delegateCount=0,f.setup&&!1!==f.setup.call(t,r,h,a)||t.addEventListener&&t.addEventListener(d,a)),f.add&&(f.add.call(t,c),c.handler.guid||(c.handler.guid=n.guid)),i?p.splice(p.delegateCount++,0,c):p.push(c),ce.event.global[d]=!0)}},remove:function(e,t,n,r,i){var o,a,s,u,l,c,f,p,d,h,g,v=_.hasData(e)&&_.get(e);if(v&&(u=v.events)){l=(t=(t||\"\").match(D)||[\"\"]).length;while(l--)if(d=g=(s=De.exec(t[l])||[])[1],h=(s[2]||\"\").split(\".\").sort(),d){f=ce.event.special[d]||{},p=u[d=(r?f.delegateType:f.bindType)||d]||[],s=s[2]&&new RegExp(\"(^|\\\\.)\"+h.join(\"\\\\.(?:.*\\\\.|)\")+\"(\\\\.|$)\"),a=o=p.length;while(o--)c=p[o],!i&&g!==c.origType||n&&n.guid!==c.guid||s&&!s.test(c.namespace)||r&&r!==c.selector&&(\"**\"!==r||!c.selector)||(p.splice(o,1),c.selector&&p.delegateCount--,f.remove&&f.remove.call(e,c));a&&!p.length&&(f.teardown&&!1!==f.teardown.call(e,h,v.handle)||ce.removeEvent(e,d,v.handle),delete u[d])}else for(d in u)ce.event.remove(e,d+t[l],n,r,!0);ce.isEmptyObject(u)&&_.remove(e,\"handle events\")}},dispatch:function(e){var t,n,r,i,o,a,s=new Array(arguments.length),u=ce.event.fix(e),l=(_.get(this,\"events\")||Object.create(null))[u.type]||[],c=ce.event.special[u.type]||{};for(s[0]=u,t=1;t<arguments.length;t++)s[t]=arguments[t];if(u.delegateTarget=this,!c.preDispatch||!1!==c.preDispatch.call(this,u)){a=ce.event.handlers.call(this,u,l),t=0;while((i=a[t++])&&!u.isPropagationStopped()){u.currentTarget=i.elem,n=0;while((o=i.handlers[n++])&&!u.isImmediatePropagationStopped())u.rnamespace&&!1!==o.namespace&&!u.rnamespace.test(o.namespace)||(u.handleObj=o,u.data=o.data,void 0!==(r=((ce.event.special[o.origType]||{}).handle||o.handler).apply(i.elem,s))&&!1===(u.result=r)&&(u.preventDefault(),u.stopPropagation()))}return c.postDispatch&&c.postDispatch.call(this,u),u.result}},handlers:function(e,t){var n,r,i,o,a,s=[],u=t.delegateCount,l=e.target;if(u&&l.nodeType&&!(\"click\"===e.type&&1<=e.button))for(;l!==this;l=l.parentNode||this)if(1===l.nodeType&&(\"click\"!==e.type||!0!==l.disabled)){for(o=[],a={},n=0;n<u;n++)void 0===a[i=(r=t[n]).selector+\" \"]&&(a[i]=r.needsContext?-1<ce(i,this).index(l):ce.find(i,this,null,[l]).length),a[i]&&o.push(r);o.length&&s.push({elem:l,handlers:o})}return l=this,u<t.length&&s.push({elem:l,handlers:t.slice(u)}),s},addProp:function(t,e){Object.defineProperty(ce.Event.prototype,t,{enumerable:!0,configurable:!0,get:v(e)?function(){if(this.originalEvent)return e(this.originalEvent)}:function(){if(this.originalEvent)return this.originalEvent[t]},set:function(e){Object.defineProperty(this,t,{enumerable:!0,configurable:!0,writable:!0,value:e})}})},fix:function(e){return e[ce.expando]?e:new ce.Event(e)},special:{load:{noBubble:!0},click:{setup:function(e){var t=this||e;return we.test(t.type)&&t.click&&fe(t,\"input\")&&He(t,\"click\",!0),!1},trigger:function(e){var t=this||e;return we.test(t.type)&&t.click&&fe(t,\"input\")&&He(t,\"click\"),!0},_default:function(e){var t=e.target;return we.test(t.type)&&t.click&&fe(t,\"input\")&&_.get(t,\"click\")||fe(t,\"a\")}},beforeunload:{postDispatch:function(e){void 0!==e.result&&e.originalEvent&&(e.originalEvent.returnValue=e.result)}}}},ce.removeEvent=function(e,t,n){e.removeEventListener&&e.removeEventListener(t,n)},ce.Event=function(e,t){if(!(this instanceof ce.Event))return new ce.Event(e,t);e&&e.type?(this.originalEvent=e,this.type=e.type,this.isDefaultPrevented=e.defaultPrevented||void 0===e.defaultPrevented&&!1===e.returnValue?Ne:qe,this.target=e.target&&3===e.target.nodeType?e.target.parentNode:e.target,this.currentTarget=e.currentTarget,this.relatedTarget=e.relatedTarget):this.type=e,t&&ce.extend(this,t),this.timeStamp=e&&e.timeStamp||Date.now(),this[ce.expando]=!0},ce.Event.prototype={constructor:ce.Event,isDefaultPrevented:qe,isPropagationStopped:qe,isImmediatePropagationStopped:qe,isSimulated:!1,preventDefault:function(){var e=this.originalEvent;this.isDefaultPrevented=Ne,e&&!this.isSimulated&&e.preventDefault()},stopPropagation:function(){var e=this.originalEvent;this.isPropagationStopped=Ne,e&&!this.isSimulated&&e.stopPropagation()},stopImmediatePropagation:function(){var e=this.originalEvent;this.isImmediatePropagationStopped=Ne,e&&!this.isSimulated&&e.stopImmediatePropagation(),this.stopPropagation()}},ce.each({altKey:!0,bubbles:!0,cancelable:!0,changedTouches:!0,ctrlKey:!0,detail:!0,eventPhase:!0,metaKey:!0,pageX:!0,pageY:!0,shiftKey:!0,view:!0,\"char\":!0,code:!0,charCode:!0,key:!0,keyCode:!0,button:!0,buttons:!0,clientX:!0,clientY:!0,offsetX:!0,offsetY:!0,pointerId:!0,pointerType:!0,screenX:!0,screenY:!0,targetTouches:!0,toElement:!0,touches:!0,which:!0},ce.event.addProp),ce.each({focus:\"focusin\",blur:\"focusout\"},function(r,i){function o(e){if(C.documentMode){var t=_.get(this,\"handle\"),n=ce.event.fix(e);n.type=\"focusin\"===e.type?\"focus\":\"blur\",n.isSimulated=!0,t(e),n.target===n.currentTarget&&t(n)}else ce.event.simulate(i,e.target,ce.event.fix(e))}ce.event.special[r]={setup:function(){var e;if(He(this,r,!0),!C.documentMode)return!1;(e=_.get(this,i))||this.addEventListener(i,o),_.set(this,i,(e||0)+1)},trigger:function(){return He(this,r),!0},teardown:function(){var e;if(!C.documentMode)return!1;(e=_.get(this,i)-1)?_.set(this,i,e):(this.removeEventListener(i,o),_.remove(this,i))},_default:function(e){return _.get(e.target,r)},delegateType:i},ce.event.special[i]={setup:function(){var e=this.ownerDocument||this.document||this,t=C.documentMode?this:e,n=_.get(t,i);n||(C.documentMode?this.addEventListener(i,o):e.addEventListener(r,o,!0)),_.set(t,i,(n||0)+1)},teardown:function(){var e=this.ownerDocument||this.document||this,t=C.documentMode?this:e,n=_.get(t,i)-1;n?_.set(t,i,n):(C.documentMode?this.removeEventListener(i,o):e.removeEventListener(r,o,!0),_.remove(t,i))}}}),ce.each({mouseenter:\"mouseover\",mouseleave:\"mouseout\",pointerenter:\"pointerover\",pointerleave:\"pointerout\"},function(e,i){ce.event.special[e]={delegateType:i,bindType:i,handle:function(e){var t,n=e.relatedTarget,r=e.handleObj;return n&&(n===this||ce.contains(this,n))||(e.type=r.origType,t=r.handler.apply(this,arguments),e.type=i),t}}}),ce.fn.extend({on:function(e,t,n,r){return Le(this,e,t,n,r)},one:function(e,t,n,r){return Le(this,e,t,n,r,1)},off:function(e,t,n){var r,i;if(e&&e.preventDefault&&e.handleObj)return r=e.handleObj,ce(e.delegateTarget).off(r.namespace?r.origType+\".\"+r.namespace:r.origType,r.selector,r.handler),this;if(\"object\"==typeof e){for(i in e)this.off(i,t,e[i]);return this}return!1!==t&&\"function\"!=typeof t||(n=t,t=void 0),!1===n&&(n=qe),this.each(function(){ce.event.remove(this,e,n,t)})}});var Oe=/<script|<style|<link/i,Pe=/checked\\s*(?:[^=]|=\\s*.checked.)/i,Me=/^\\s*<!\\[CDATA\\[|\\]\\]>\\s*$/g;function Re(e,t){return fe(e,\"table\")&&fe(11!==t.nodeType?t:t.firstChild,\"tr\")&&ce(e).children(\"tbody\")[0]||e}function Ie(e){return e.type=(null!==e.getAttribute(\"type\"))+\"/\"+e.type,e}function We(e){return\"true/\"===(e.type||\"\").slice(0,5)?e.type=e.type.slice(5):e.removeAttribute(\"type\"),e}function Fe(e,t){var n,r,i,o,a,s;if(1===t.nodeType){if(_.hasData(e)&&(s=_.get(e).events))for(i in _.remove(t,\"handle events\"),s)for(n=0,r=s[i].length;n<r;n++)ce.event.add(t,i,s[i][n]);z.hasData(e)&&(o=z.access(e),a=ce.extend({},o),z.set(t,a))}}function $e(n,r,i,o){r=g(r);var e,t,a,s,u,l,c=0,f=n.length,p=f-1,d=r[0],h=v(d);if(h||1<f&&\"string\"==typeof d&&!le.checkClone&&Pe.test(d))return n.each(function(e){var t=n.eq(e);h&&(r[0]=d.call(this,e,t.html())),$e(t,r,i,o)});if(f&&(t=(e=Ae(r,n[0].ownerDocument,!1,n,o)).firstChild,1===e.childNodes.length&&(e=t),t||o)){for(s=(a=ce.map(Se(e,\"script\"),Ie)).length;c<f;c++)u=e,c!==p&&(u=ce.clone(u,!0,!0),s&&ce.merge(a,Se(u,\"script\"))),i.call(n[c],u,c);if(s)for(l=a[a.length-1].ownerDocument,ce.map(a,We),c=0;c<s;c++)u=a[c],Ce.test(u.type||\"\")&&!_.access(u,\"globalEval\")&&ce.contains(l,u)&&(u.src&&\"module\"!==(u.type||\"\").toLowerCase()?ce._evalUrl&&!u.noModule&&ce._evalUrl(u.src,{nonce:u.nonce||u.getAttribute(\"nonce\")},l):m(u.textContent.replace(Me,\"\"),u,l))}return n}function Be(e,t,n){for(var r,i=t?ce.filter(t,e):e,o=0;null!=(r=i[o]);o++)n||1!==r.nodeType||ce.cleanData(Se(r)),r.parentNode&&(n&&K(r)&&Ee(Se(r,\"script\")),r.parentNode.removeChild(r));return e}ce.extend({htmlPrefilter:function(e){return e},clone:function(e,t,n){var r,i,o,a,s,u,l,c=e.cloneNode(!0),f=K(e);if(!(le.noCloneChecked||1!==e.nodeType&&11!==e.nodeType||ce.isXMLDoc(e)))for(a=Se(c),r=0,i=(o=Se(e)).length;r<i;r++)s=o[r],u=a[r],void 0,\"input\"===(l=u.nodeName.toLowerCase())&&we.test(s.type)?u.checked=s.checked:\"input\"!==l&&\"textarea\"!==l||(u.defaultValue=s.defaultValue);if(t)if(n)for(o=o||Se(e),a=a||Se(c),r=0,i=o.length;r<i;r++)Fe(o[r],a[r]);else Fe(e,c);return 0<(a=Se(c,\"script\")).length&&Ee(a,!f&&Se(e,\"script\")),c},cleanData:function(e){for(var t,n,r,i=ce.event.special,o=0;void 0!==(n=e[o]);o++)if($(n)){if(t=n[_.expando]){if(t.events)for(r in t.events)i[r]?ce.event.remove(n,r):ce.removeEvent(n,r,t.handle);n[_.expando]=void 0}n[z.expando]&&(n[z.expando]=void 0)}}}),ce.fn.extend({detach:function(e){return Be(this,e,!0)},remove:function(e){return Be(this,e)},text:function(e){return M(this,function(e){return void 0===e?ce.text(this):this.empty().each(function(){1!==this.nodeType&&11!==this.nodeType&&9!==this.nodeType||(this.textContent=e)})},null,e,arguments.length)},append:function(){return $e(this,arguments,function(e){1!==this.nodeType&&11!==this.nodeType&&9!==this.nodeType||Re(this,e).appendChild(e)})},prepend:function(){return $e(this,arguments,function(e){if(1===this.nodeType||11===this.nodeType||9===this.nodeType){var t=Re(this,e);t.insertBefore(e,t.firstChild)}})},before:function(){return $e(this,arguments,function(e){this.parentNode&&this.parentNode.insertBefore(e,this)})},after:function(){return $e(this,arguments,function(e){this.parentNode&&this.parentNode.insertBefore(e,this.nextSibling)})},empty:function(){for(var e,t=0;null!=(e=this[t]);t++)1===e.nodeType&&(ce.cleanData(Se(e,!1)),e.textContent=\"\");return this},clone:function(e,t){return e=null!=e&&e,t=null==t?e:t,this.map(function(){return ce.clone(this,e,t)})},html:function(e){return M(this,function(e){var t=this[0]||{},n=0,r=this.length;if(void 0===e&&1===t.nodeType)return t.innerHTML;if(\"string\"==typeof e&&!Oe.test(e)&&!ke[(Te.exec(e)||[\"\",\"\"])[1].toLowerCase()]){e=ce.htmlPrefilter(e);try{for(;n<r;n++)1===(t=this[n]||{}).nodeType&&(ce.cleanData(Se(t,!1)),t.innerHTML=e);t=0}catch(e){}}t&&this.empty().append(e)},null,e,arguments.length)},replaceWith:function(){var n=[];return $e(this,arguments,function(e){var t=this.parentNode;ce.inArray(this,n)<0&&(ce.cleanData(Se(this)),t&&t.replaceChild(e,this))},n)}}),ce.each({appendTo:\"append\",prependTo:\"prepend\",insertBefore:\"before\",insertAfter:\"after\",replaceAll:\"replaceWith\"},function(e,a){ce.fn[e]=function(e){for(var t,n=[],r=ce(e),i=r.length-1,o=0;o<=i;o++)t=o===i?this:this.clone(!0),ce(r[o])[a](t),s.apply(n,t.get());return this.pushStack(n)}});var _e=new RegExp(\"^(\"+G+\")(?!px)[a-z%]+$\",\"i\"),ze=/^--/,Xe=function(e){var t=e.ownerDocument.defaultView;return t&&t.opener||(t=ie),t.getComputedStyle(e)},Ue=function(e,t,n){var r,i,o={};for(i in t)o[i]=e.style[i],e.style[i]=t[i];for(i in r=n.call(e),t)e.style[i]=o[i];return r},Ve=new RegExp(Q.join(\"|\"),\"i\");function Ge(e,t,n){var r,i,o,a,s=ze.test(t),u=e.style;return(n=n||Xe(e))&&(a=n.getPropertyValue(t)||n[t],s&&a&&(a=a.replace(ve,\"$1\")||void 0),\"\"!==a||K(e)||(a=ce.style(e,t)),!le.pixelBoxStyles()&&_e.test(a)&&Ve.test(t)&&(r=u.width,i=u.minWidth,o=u.maxWidth,u.minWidth=u.maxWidth=u.width=a,a=n.width,u.width=r,u.minWidth=i,u.maxWidth=o)),void 0!==a?a+\"\":a}function Ye(e,t){return{get:function(){if(!e())return(this.get=t).apply(this,arguments);delete this.get}}}!function(){function e(){if(l){u.style.cssText=\"position:absolute;left:-11111px;width:60px;margin-top:1px;padding:0;border:0\",l.style.cssText=\"position:relative;display:block;box-sizing:border-box;overflow:scroll;margin:auto;border:1px;padding:1px;width:60%;top:1%\",J.appendChild(u).appendChild(l);var e=ie.getComputedStyle(l);n=\"1%\"!==e.top,s=12===t(e.marginLeft),l.style.right=\"60%\",o=36===t(e.right),r=36===t(e.width),l.style.position=\"absolute\",i=12===t(l.offsetWidth/3),J.removeChild(u),l=null}}function t(e){return Math.round(parseFloat(e))}var n,r,i,o,a,s,u=C.createElement(\"div\"),l=C.createElement(\"div\");l.style&&(l.style.backgroundClip=\"content-box\",l.cloneNode(!0).style.backgroundClip=\"\",le.clearCloneStyle=\"content-box\"===l.style.backgroundClip,ce.extend(le,{boxSizingReliable:function(){return e(),r},pixelBoxStyles:function(){return e(),o},pixelPosition:function(){return e(),n},reliableMarginLeft:function(){return e(),s},scrollboxSize:function(){return e(),i},reliableTrDimensions:function(){var e,t,n,r;return null==a&&(e=C.createElement(\"table\"),t=C.createElement(\"tr\"),n=C.createElement(\"div\"),e.style.cssText=\"position:absolute;left:-11111px;border-collapse:separate\",t.style.cssText=\"box-sizing:content-box;border:1px solid\",t.style.height=\"1px\",n.style.height=\"9px\",n.style.display=\"block\",J.appendChild(e).appendChild(t).appendChild(n),r=ie.getComputedStyle(t),a=parseInt(r.height,10)+parseInt(r.borderTopWidth,10)+parseInt(r.borderBottomWidth,10)===t.offsetHeight,J.removeChild(e)),a}}))}();var Qe=[\"Webkit\",\"Moz\",\"ms\"],Je=C.createElement(\"div\").style,Ke={};function Ze(e){var t=ce.cssProps[e]||Ke[e];return t||(e in Je?e:Ke[e]=function(e){var t=e[0].toUpperCase()+e.slice(1),n=Qe.length;while(n--)if((e=Qe[n]+t)in Je)return e}(e)||e)}var et=/^(none|table(?!-c[ea]).+)/,tt={position:\"absolute\",visibility:\"hidden\",display:\"block\"},nt={letterSpacing:\"0\",fontWeight:\"400\"};function rt(e,t,n){var r=Y.exec(t);return r?Math.max(0,r[2]-(n||0))+(r[3]||\"px\"):t}function it(e,t,n,r,i,o){var a=\"width\"===t?1:0,s=0,u=0,l=0;if(n===(r?\"border\":\"content\"))return 0;for(;a<4;a+=2)\"margin\"===n&&(l+=ce.css(e,n+Q[a],!0,i)),r?(\"content\"===n&&(u-=ce.css(e,\"padding\"+Q[a],!0,i)),\"margin\"!==n&&(u-=ce.css(e,\"border\"+Q[a]+\"Width\",!0,i))):(u+=ce.css(e,\"padding\"+Q[a],!0,i),\"padding\"!==n?u+=ce.css(e,\"border\"+Q[a]+\"Width\",!0,i):s+=ce.css(e,\"border\"+Q[a]+\"Width\",!0,i));return!r&&0<=o&&(u+=Math.max(0,Math.ceil(e[\"offset\"+t[0].toUpperCase()+t.slice(1)]-o-u-s-.5))||0),u+l}function ot(e,t,n){var r=Xe(e),i=(!le.boxSizingReliable()||n)&&\"border-box\"===ce.css(e,\"boxSizing\",!1,r),o=i,a=Ge(e,t,r),s=\"offset\"+t[0].toUpperCase()+t.slice(1);if(_e.test(a)){if(!n)return a;a=\"auto\"}return(!le.boxSizingReliable()&&i||!le.reliableTrDimensions()&&fe(e,\"tr\")||\"auto\"===a||!parseFloat(a)&&\"inline\"===ce.css(e,\"display\",!1,r))&&e.getClientRects().length&&(i=\"border-box\"===ce.css(e,\"boxSizing\",!1,r),(o=s in e)&&(a=e[s])),(a=parseFloat(a)||0)+it(e,t,n||(i?\"border\":\"content\"),o,r,a)+\"px\"}function at(e,t,n,r,i){return new at.prototype.init(e,t,n,r,i)}ce.extend({cssHooks:{opacity:{get:function(e,t){if(t){var n=Ge(e,\"opacity\");return\"\"===n?\"1\":n}}}},cssNumber:{animationIterationCount:!0,aspectRatio:!0,borderImageSlice:!0,columnCount:!0,flexGrow:!0,flexShrink:!0,fontWeight:!0,gridArea:!0,gridColumn:!0,gridColumnEnd:!0,gridColumnStart:!0,gridRow:!0,gridRowEnd:!0,gridRowStart:!0,lineHeight:!0,opacity:!0,order:!0,orphans:!0,scale:!0,widows:!0,zIndex:!0,zoom:!0,fillOpacity:!0,floodOpacity:!0,stopOpacity:!0,strokeMiterlimit:!0,strokeOpacity:!0},cssProps:{},style:function(e,t,n,r){if(e&&3!==e.nodeType&&8!==e.nodeType&&e.style){var i,o,a,s=F(t),u=ze.test(t),l=e.style;if(u||(t=Ze(s)),a=ce.cssHooks[t]||ce.cssHooks[s],void 0===n)return a&&\"get\"in a&&void 0!==(i=a.get(e,!1,r))?i:l[t];\"string\"===(o=typeof n)&&(i=Y.exec(n))&&i[1]&&(n=te(e,t,i),o=\"number\"),null!=n&&n==n&&(\"number\"!==o||u||(n+=i&&i[3]||(ce.cssNumber[s]?\"\":\"px\")),le.clearCloneStyle||\"\"!==n||0!==t.indexOf(\"background\")||(l[t]=\"inherit\"),a&&\"set\"in a&&void 0===(n=a.set(e,n,r))||(u?l.setProperty(t,n):l[t]=n))}},css:function(e,t,n,r){var i,o,a,s=F(t);return ze.test(t)||(t=Ze(s)),(a=ce.cssHooks[t]||ce.cssHooks[s])&&\"get\"in a&&(i=a.get(e,!0,n)),void 0===i&&(i=Ge(e,t,r)),\"normal\"===i&&t in nt&&(i=nt[t]),\"\"===n||n?(o=parseFloat(i),!0===n||isFinite(o)?o||0:i):i}}),ce.each([\"height\",\"width\"],function(e,u){ce.cssHooks[u]={get:function(e,t,n){if(t)return!et.test(ce.css(e,\"display\"))||e.getClientRects().length&&e.getBoundingClientRect().width?ot(e,u,n):Ue(e,tt,function(){return ot(e,u,n)})},set:function(e,t,n){var r,i=Xe(e),o=!le.scrollboxSize()&&\"absolute\"===i.position,a=(o||n)&&\"border-box\"===ce.css(e,\"boxSizing\",!1,i),s=n?it(e,u,n,a,i):0;return a&&o&&(s-=Math.ceil(e[\"offset\"+u[0].toUpperCase()+u.slice(1)]-parseFloat(i[u])-it(e,u,\"border\",!1,i)-.5)),s&&(r=Y.exec(t))&&\"px\"!==(r[3]||\"px\")&&(e.style[u]=t,t=ce.css(e,u)),rt(0,t,s)}}}),ce.cssHooks.marginLeft=Ye(le.reliableMarginLeft,function(e,t){if(t)return(parseFloat(Ge(e,\"marginLeft\"))||e.getBoundingClientRect().left-Ue(e,{marginLeft:0},function(){return e.getBoundingClientRect().left}))+\"px\"}),ce.each({margin:\"\",padding:\"\",border:\"Width\"},function(i,o){ce.cssHooks[i+o]={expand:function(e){for(var t=0,n={},r=\"string\"==typeof e?e.split(\" \"):[e];t<4;t++)n[i+Q[t]+o]=r[t]||r[t-2]||r[0];return n}},\"margin\"!==i&&(ce.cssHooks[i+o].set=rt)}),ce.fn.extend({css:function(e,t){return M(this,function(e,t,n){var r,i,o={},a=0;if(Array.isArray(t)){for(r=Xe(e),i=t.length;a<i;a++)o[t[a]]=ce.css(e,t[a],!1,r);return o}return void 0!==n?ce.style(e,t,n):ce.css(e,t)},e,t,1<arguments.length)}}),((ce.Tween=at).prototype={constructor:at,init:function(e,t,n,r,i,o){this.elem=e,this.prop=n,this.easing=i||ce.easing._default,this.options=t,this.start=this.now=this.cur(),this.end=r,this.unit=o||(ce.cssNumber[n]?\"\":\"px\")},cur:function(){var e=at.propHooks[this.prop];return e&&e.get?e.get(this):at.propHooks._default.get(this)},run:function(e){var t,n=at.propHooks[this.prop];return this.options.duration?this.pos=t=ce.easing[this.easing](e,this.options.duration*e,0,1,this.options.duration):this.pos=t=e,this.now=(this.end-this.start)*t+this.start,this.options.step&&this.options.step.call(this.elem,this.now,this),n&&n.set?n.set(this):at.propHooks._default.set(this),this}}).init.prototype=at.prototype,(at.propHooks={_default:{get:function(e){var t;return 1!==e.elem.nodeType||null!=e.elem[e.prop]&&null==e.elem.style[e.prop]?e.elem[e.prop]:(t=ce.css(e.elem,e.prop,\"\"))&&\"auto\"!==t?t:0},set:function(e){ce.fx.step[e.prop]?ce.fx.step[e.prop](e):1!==e.elem.nodeType||!ce.cssHooks[e.prop]&&null==e.elem.style[Ze(e.prop)]?e.elem[e.prop]=e.now:ce.style(e.elem,e.prop,e.now+e.unit)}}}).scrollTop=at.propHooks.scrollLeft={set:function(e){e.elem.nodeType&&e.elem.parentNode&&(e.elem[e.prop]=e.now)}},ce.easing={linear:function(e){return e},swing:function(e){return.5-Math.cos(e*Math.PI)/2},_default:\"swing\"},ce.fx=at.prototype.init,ce.fx.step={};var st,ut,lt,ct,ft=/^(?:toggle|show|hide)$/,pt=/queueHooks$/;function dt(){ut&&(!1===C.hidden&&ie.requestAnimationFrame?ie.requestAnimationFrame(dt):ie.setTimeout(dt,ce.fx.interval),ce.fx.tick())}function ht(){return ie.setTimeout(function(){st=void 0}),st=Date.now()}function gt(e,t){var n,r=0,i={height:e};for(t=t?1:0;r<4;r+=2-t)i[\"margin\"+(n=Q[r])]=i[\"padding\"+n]=e;return t&&(i.opacity=i.width=e),i}function vt(e,t,n){for(var r,i=(yt.tweeners[t]||[]).concat(yt.tweeners[\"*\"]),o=0,a=i.length;o<a;o++)if(r=i[o].call(n,t,e))return r}function yt(o,e,t){var n,a,r=0,i=yt.prefilters.length,s=ce.Deferred().always(function(){delete u.elem}),u=function(){if(a)return!1;for(var e=st||ht(),t=Math.max(0,l.startTime+l.duration-e),n=1-(t/l.duration||0),r=0,i=l.tweens.length;r<i;r++)l.tweens[r].run(n);return s.notifyWith(o,[l,n,t]),n<1&&i?t:(i||s.notifyWith(o,[l,1,0]),s.resolveWith(o,[l]),!1)},l=s.promise({elem:o,props:ce.extend({},e),opts:ce.extend(!0,{specialEasing:{},easing:ce.easing._default},t),originalProperties:e,originalOptions:t,startTime:st||ht(),duration:t.duration,tweens:[],createTween:function(e,t){var n=ce.Tween(o,l.opts,e,t,l.opts.specialEasing[e]||l.opts.easing);return l.tweens.push(n),n},stop:function(e){var t=0,n=e?l.tweens.length:0;if(a)return this;for(a=!0;t<n;t++)l.tweens[t].run(1);return e?(s.notifyWith(o,[l,1,0]),s.resolveWith(o,[l,e])):s.rejectWith(o,[l,e]),this}}),c=l.props;for(!function(e,t){var n,r,i,o,a;for(n in e)if(i=t[r=F(n)],o=e[n],Array.isArray(o)&&(i=o[1],o=e[n]=o[0]),n!==r&&(e[r]=o,delete e[n]),(a=ce.cssHooks[r])&&\"expand\"in a)for(n in o=a.expand(o),delete e[r],o)n in e||(e[n]=o[n],t[n]=i);else t[r]=i}(c,l.opts.specialEasing);r<i;r++)if(n=yt.prefilters[r].call(l,o,c,l.opts))return v(n.stop)&&(ce._queueHooks(l.elem,l.opts.queue).stop=n.stop.bind(n)),n;return ce.map(c,vt,l),v(l.opts.start)&&l.opts.start.call(o,l),l.progress(l.opts.progress).done(l.opts.done,l.opts.complete).fail(l.opts.fail).always(l.opts.always),ce.fx.timer(ce.extend(u,{elem:o,anim:l,queue:l.opts.queue})),l}ce.Animation=ce.extend(yt,{tweeners:{\"*\":[function(e,t){var n=this.createTween(e,t);return te(n.elem,e,Y.exec(t),n),n}]},tweener:function(e,t){v(e)?(t=e,e=[\"*\"]):e=e.match(D);for(var n,r=0,i=e.length;r<i;r++)n=e[r],yt.tweeners[n]=yt.tweeners[n]||[],yt.tweeners[n].unshift(t)},prefilters:[function(e,t,n){var r,i,o,a,s,u,l,c,f=\"width\"in t||\"height\"in t,p=this,d={},h=e.style,g=e.nodeType&&ee(e),v=_.get(e,\"fxshow\");for(r in n.queue||(null==(a=ce._queueHooks(e,\"fx\")).unqueued&&(a.unqueued=0,s=a.empty.fire,a.empty.fire=function(){a.unqueued||s()}),a.unqueued++,p.always(function(){p.always(function(){a.unqueued--,ce.queue(e,\"fx\").length||a.empty.fire()})})),t)if(i=t[r],ft.test(i)){if(delete t[r],o=o||\"toggle\"===i,i===(g?\"hide\":\"show\")){if(\"show\"!==i||!v||void 0===v[r])continue;g=!0}d[r]=v&&v[r]||ce.style(e,r)}if((u=!ce.isEmptyObject(t))||!ce.isEmptyObject(d))for(r in f&&1===e.nodeType&&(n.overflow=[h.overflow,h.overflowX,h.overflowY],null==(l=v&&v.display)&&(l=_.get(e,\"display\")),\"none\"===(c=ce.css(e,\"display\"))&&(l?c=l:(re([e],!0),l=e.style.display||l,c=ce.css(e,\"display\"),re([e]))),(\"inline\"===c||\"inline-block\"===c&&null!=l)&&\"none\"===ce.css(e,\"float\")&&(u||(p.done(function(){h.display=l}),null==l&&(c=h.display,l=\"none\"===c?\"\":c)),h.display=\"inline-block\")),n.overflow&&(h.overflow=\"hidden\",p.always(function(){h.overflow=n.overflow[0],h.overflowX=n.overflow[1],h.overflowY=n.overflow[2]})),u=!1,d)u||(v?\"hidden\"in v&&(g=v.hidden):v=_.access(e,\"fxshow\",{display:l}),o&&(v.hidden=!g),g&&re([e],!0),p.done(function(){for(r in g||re([e]),_.remove(e,\"fxshow\"),d)ce.style(e,r,d[r])})),u=vt(g?v[r]:0,r,p),r in v||(v[r]=u.start,g&&(u.end=u.start,u.start=0))}],prefilter:function(e,t){t?yt.prefilters.unshift(e):yt.prefilters.push(e)}}),ce.speed=function(e,t,n){var r=e&&\"object\"==typeof e?ce.extend({},e):{complete:n||!n&&t||v(e)&&e,duration:e,easing:n&&t||t&&!v(t)&&t};return ce.fx.off?r.duration=0:\"number\"!=typeof r.duration&&(r.duration in ce.fx.speeds?r.duration=ce.fx.speeds[r.duration]:r.duration=ce.fx.speeds._default),null!=r.queue&&!0!==r.queue||(r.queue=\"fx\"),r.old=r.complete,r.complete=function(){v(r.old)&&r.old.call(this),r.queue&&ce.dequeue(this,r.queue)},r},ce.fn.extend({fadeTo:function(e,t,n,r){return this.filter(ee).css(\"opacity\",0).show().end().animate({opacity:t},e,n,r)},animate:function(t,e,n,r){var i=ce.isEmptyObject(t),o=ce.speed(e,n,r),a=function(){var e=yt(this,ce.extend({},t),o);(i||_.get(this,\"finish\"))&&e.stop(!0)};return a.finish=a,i||!1===o.queue?this.each(a):this.queue(o.queue,a)},stop:function(i,e,o){var a=function(e){var t=e.stop;delete e.stop,t(o)};return\"string\"!=typeof i&&(o=e,e=i,i=void 0),e&&this.queue(i||\"fx\",[]),this.each(function(){var e=!0,t=null!=i&&i+\"queueHooks\",n=ce.timers,r=_.get(this);if(t)r[t]&&r[t].stop&&a(r[t]);else for(t in r)r[t]&&r[t].stop&&pt.test(t)&&a(r[t]);for(t=n.length;t--;)n[t].elem!==this||null!=i&&n[t].queue!==i||(n[t].anim.stop(o),e=!1,n.splice(t,1));!e&&o||ce.dequeue(this,i)})},finish:function(a){return!1!==a&&(a=a||\"fx\"),this.each(function(){var e,t=_.get(this),n=t[a+\"queue\"],r=t[a+\"queueHooks\"],i=ce.timers,o=n?n.length:0;for(t.finish=!0,ce.queue(this,a,[]),r&&r.stop&&r.stop.call(this,!0),e=i.length;e--;)i[e].elem===this&&i[e].queue===a&&(i[e].anim.stop(!0),i.splice(e,1));for(e=0;e<o;e++)n[e]&&n[e].finish&&n[e].finish.call(this);delete t.finish})}}),ce.each([\"toggle\",\"show\",\"hide\"],function(e,r){var i=ce.fn[r];ce.fn[r]=function(e,t,n){return null==e||\"boolean\"==typeof e?i.apply(this,arguments):this.animate(gt(r,!0),e,t,n)}}),ce.each({slideDown:gt(\"show\"),slideUp:gt(\"hide\"),slideToggle:gt(\"toggle\"),fadeIn:{opacity:\"show\"},fadeOut:{opacity:\"hide\"},fadeToggle:{opacity:\"toggle\"}},function(e,r){ce.fn[e]=function(e,t,n){return this.animate(r,e,t,n)}}),ce.timers=[],ce.fx.tick=function(){var e,t=0,n=ce.timers;for(st=Date.now();t<n.length;t++)(e=n[t])()||n[t]!==e||n.splice(t--,1);n.length||ce.fx.stop(),st=void 0},ce.fx.timer=function(e){ce.timers.push(e),ce.fx.start()},ce.fx.interval=13,ce.fx.start=function(){ut||(ut=!0,dt())},ce.fx.stop=function(){ut=null},ce.fx.speeds={slow:600,fast:200,_default:400},ce.fn.delay=function(r,e){return r=ce.fx&&ce.fx.speeds[r]||r,e=e||\"fx\",this.queue(e,function(e,t){var n=ie.setTimeout(e,r);t.stop=function(){ie.clearTimeout(n)}})},lt=C.createElement(\"input\"),ct=C.createElement(\"select\").appendChild(C.createElement(\"option\")),lt.type=\"checkbox\",le.checkOn=\"\"!==lt.value,le.optSelected=ct.selected,(lt=C.createElement(\"input\")).value=\"t\",lt.type=\"radio\",le.radioValue=\"t\"===lt.value;var mt,xt=ce.expr.attrHandle;ce.fn.extend({attr:function(e,t){return M(this,ce.attr,e,t,1<arguments.length)},removeAttr:function(e){return this.each(function(){ce.removeAttr(this,e)})}}),ce.extend({attr:function(e,t,n){var r,i,o=e.nodeType;if(3!==o&&8!==o&&2!==o)return\"undefined\"==typeof e.getAttribute?ce.prop(e,t,n):(1===o&&ce.isXMLDoc(e)||(i=ce.attrHooks[t.toLowerCase()]||(ce.expr.match.bool.test(t)?mt:void 0)),void 0!==n?null===n?void ce.removeAttr(e,t):i&&\"set\"in i&&void 0!==(r=i.set(e,n,t))?r:(e.setAttribute(t,n+\"\"),n):i&&\"get\"in i&&null!==(r=i.get(e,t))?r:null==(r=ce.find.attr(e,t))?void 0:r)},attrHooks:{type:{set:function(e,t){if(!le.radioValue&&\"radio\"===t&&fe(e,\"input\")){var n=e.value;return e.setAttribute(\"type\",t),n&&(e.value=n),t}}}},removeAttr:function(e,t){var n,r=0,i=t&&t.match(D);if(i&&1===e.nodeType)while(n=i[r++])e.removeAttribute(n)}}),mt={set:function(e,t,n){return!1===t?ce.removeAttr(e,n):e.setAttribute(n,n),n}},ce.each(ce.expr.match.bool.source.match(/\\w+/g),function(e,t){var a=xt[t]||ce.find.attr;xt[t]=function(e,t,n){var r,i,o=t.toLowerCase();return n||(i=xt[o],xt[o]=r,r=null!=a(e,t,n)?o:null,xt[o]=i),r}});var bt=/^(?:input|select|textarea|button)$/i,wt=/^(?:a|area)$/i;function Tt(e){return(e.match(D)||[]).join(\" \")}function Ct(e){return e.getAttribute&&e.getAttribute(\"class\")||\"\"}function kt(e){return Array.isArray(e)?e:\"string\"==typeof e&&e.match(D)||[]}ce.fn.extend({prop:function(e,t){return M(this,ce.prop,e,t,1<arguments.length)},removeProp:function(e){return this.each(function(){delete this[ce.propFix[e]||e]})}}),ce.extend({prop:function(e,t,n){var r,i,o=e.nodeType;if(3!==o&&8!==o&&2!==o)return 1===o&&ce.isXMLDoc(e)||(t=ce.propFix[t]||t,i=ce.propHooks[t]),void 0!==n?i&&\"set\"in i&&void 0!==(r=i.set(e,n,t))?r:e[t]=n:i&&\"get\"in i&&null!==(r=i.get(e,t))?r:e[t]},propHooks:{tabIndex:{get:function(e){var t=ce.find.attr(e,\"tabindex\");return t?parseInt(t,10):bt.test(e.nodeName)||wt.test(e.nodeName)&&e.href?0:-1}}},propFix:{\"for\":\"htmlFor\",\"class\":\"className\"}}),le.optSelected||(ce.propHooks.selected={get:function(e){var t=e.parentNode;return t&&t.parentNode&&t.parentNode.selectedIndex,null},set:function(e){var t=e.parentNode;t&&(t.selectedIndex,t.parentNode&&t.parentNode.selectedIndex)}}),ce.each([\"tabIndex\",\"readOnly\",\"maxLength\",\"cellSpacing\",\"cellPadding\",\"rowSpan\",\"colSpan\",\"useMap\",\"frameBorder\",\"contentEditable\"],function(){ce.propFix[this.toLowerCase()]=this}),ce.fn.extend({addClass:function(t){var e,n,r,i,o,a;return v(t)?this.each(function(e){ce(this).addClass(t.call(this,e,Ct(this)))}):(e=kt(t)).length?this.each(function(){if(r=Ct(this),n=1===this.nodeType&&\" \"+Tt(r)+\" \"){for(o=0;o<e.length;o++)i=e[o],n.indexOf(\" \"+i+\" \")<0&&(n+=i+\" \");a=Tt(n),r!==a&&this.setAttribute(\"class\",a)}}):this},removeClass:function(t){var e,n,r,i,o,a;return v(t)?this.each(function(e){ce(this).removeClass(t.call(this,e,Ct(this)))}):arguments.length?(e=kt(t)).length?this.each(function(){if(r=Ct(this),n=1===this.nodeType&&\" \"+Tt(r)+\" \"){for(o=0;o<e.length;o++){i=e[o];while(-1<n.indexOf(\" \"+i+\" \"))n=n.replace(\" \"+i+\" \",\" \")}a=Tt(n),r!==a&&this.setAttribute(\"class\",a)}}):this:this.attr(\"class\",\"\")},toggleClass:function(t,n){var e,r,i,o,a=typeof t,s=\"string\"===a||Array.isArray(t);return v(t)?this.each(function(e){ce(this).toggleClass(t.call(this,e,Ct(this),n),n)}):\"boolean\"==typeof n&&s?n?this.addClass(t):this.removeClass(t):(e=kt(t),this.each(function(){if(s)for(o=ce(this),i=0;i<e.length;i++)r=e[i],o.hasClass(r)?o.removeClass(r):o.addClass(r);else void 0!==t&&\"boolean\"!==a||((r=Ct(this))&&_.set(this,\"__className__\",r),this.setAttribute&&this.setAttribute(\"class\",r||!1===t?\"\":_.get(this,\"__className__\")||\"\"))}))},hasClass:function(e){var t,n,r=0;t=\" \"+e+\" \";while(n=this[r++])if(1===n.nodeType&&-1<(\" \"+Tt(Ct(n))+\" \").indexOf(t))return!0;return!1}});var St=/\\r/g;ce.fn.extend({val:function(n){var r,e,i,t=this[0];return arguments.length?(i=v(n),this.each(function(e){var t;1===this.nodeType&&(null==(t=i?n.call(this,e,ce(this).val()):n)?t=\"\":\"number\"==typeof t?t+=\"\":Array.isArray(t)&&(t=ce.map(t,function(e){return null==e?\"\":e+\"\"})),(r=ce.valHooks[this.type]||ce.valHooks[this.nodeName.toLowerCase()])&&\"set\"in r&&void 0!==r.set(this,t,\"value\")||(this.value=t))})):t?(r=ce.valHooks[t.type]||ce.valHooks[t.nodeName.toLowerCase()])&&\"get\"in r&&void 0!==(e=r.get(t,\"value\"))?e:\"string\"==typeof(e=t.value)?e.replace(St,\"\"):null==e?\"\":e:void 0}}),ce.extend({valHooks:{option:{get:function(e){var t=ce.find.attr(e,\"value\");return null!=t?t:Tt(ce.text(e))}},select:{get:function(e){var t,n,r,i=e.options,o=e.selectedIndex,a=\"select-one\"===e.type,s=a?null:[],u=a?o+1:i.length;for(r=o<0?u:a?o:0;r<u;r++)if(((n=i[r]).selected||r===o)&&!n.disabled&&(!n.parentNode.disabled||!fe(n.parentNode,\"optgroup\"))){if(t=ce(n).val(),a)return t;s.push(t)}return s},set:function(e,t){var n,r,i=e.options,o=ce.makeArray(t),a=i.length;while(a--)((r=i[a]).selected=-1<ce.inArray(ce.valHooks.option.get(r),o))&&(n=!0);return n||(e.selectedIndex=-1),o}}}}),ce.each([\"radio\",\"checkbox\"],function(){ce.valHooks[this]={set:function(e,t){if(Array.isArray(t))return e.checked=-1<ce.inArray(ce(e).val(),t)}},le.checkOn||(ce.valHooks[this].get=function(e){return null===e.getAttribute(\"value\")?\"on\":e.value})});var Et=ie.location,jt={guid:Date.now()},At=/\\?/;ce.parseXML=function(e){var t,n;if(!e||\"string\"!=typeof e)return null;try{t=(new ie.DOMParser).parseFromString(e,\"text/xml\")}catch(e){}return n=t&&t.getElementsByTagName(\"parsererror\")[0],t&&!n||ce.error(\"Invalid XML: \"+(n?ce.map(n.childNodes,function(e){return e.textContent}).join(\"\\n\"):e)),t};var Dt=/^(?:focusinfocus|focusoutblur)$/,Nt=function(e){e.stopPropagation()};ce.extend(ce.event,{trigger:function(e,t,n,r){var i,o,a,s,u,l,c,f,p=[n||C],d=ue.call(e,\"type\")?e.type:e,h=ue.call(e,\"namespace\")?e.namespace.split(\".\"):[];if(o=f=a=n=n||C,3!==n.nodeType&&8!==n.nodeType&&!Dt.test(d+ce.event.triggered)&&(-1<d.indexOf(\".\")&&(d=(h=d.split(\".\")).shift(),h.sort()),u=d.indexOf(\":\")<0&&\"on\"+d,(e=e[ce.expando]?e:new ce.Event(d,\"object\"==typeof e&&e)).isTrigger=r?2:3,e.namespace=h.join(\".\"),e.rnamespace=e.namespace?new RegExp(\"(^|\\\\.)\"+h.join(\"\\\\.(?:.*\\\\.|)\")+\"(\\\\.|$)\"):null,e.result=void 0,e.target||(e.target=n),t=null==t?[e]:ce.makeArray(t,[e]),c=ce.event.special[d]||{},r||!c.trigger||!1!==c.trigger.apply(n,t))){if(!r&&!c.noBubble&&!y(n)){for(s=c.delegateType||d,Dt.test(s+d)||(o=o.parentNode);o;o=o.parentNode)p.push(o),a=o;a===(n.ownerDocument||C)&&p.push(a.defaultView||a.parentWindow||ie)}i=0;while((o=p[i++])&&!e.isPropagationStopped())f=o,e.type=1<i?s:c.bindType||d,(l=(_.get(o,\"events\")||Object.create(null))[e.type]&&_.get(o,\"handle\"))&&l.apply(o,t),(l=u&&o[u])&&l.apply&&$(o)&&(e.result=l.apply(o,t),!1===e.result&&e.preventDefault());return e.type=d,r||e.isDefaultPrevented()||c._default&&!1!==c._default.apply(p.pop(),t)||!$(n)||u&&v(n[d])&&!y(n)&&((a=n[u])&&(n[u]=null),ce.event.triggered=d,e.isPropagationStopped()&&f.addEventListener(d,Nt),n[d](),e.isPropagationStopped()&&f.removeEventListener(d,Nt),ce.event.triggered=void 0,a&&(n[u]=a)),e.result}},simulate:function(e,t,n){var r=ce.extend(new ce.Event,n,{type:e,isSimulated:!0});ce.event.trigger(r,null,t)}}),ce.fn.extend({trigger:function(e,t){return this.each(function(){ce.event.trigger(e,t,this)})},triggerHandler:function(e,t){var n=this[0];if(n)return ce.event.trigger(e,t,n,!0)}});var qt=/\\[\\]$/,Lt=/\\r?\\n/g,Ht=/^(?:submit|button|image|reset|file)$/i,Ot=/^(?:input|select|textarea|keygen)/i;function Pt(n,e,r,i){var t;if(Array.isArray(e))ce.each(e,function(e,t){r||qt.test(n)?i(n,t):Pt(n+\"[\"+(\"object\"==typeof t&&null!=t?e:\"\")+\"]\",t,r,i)});else if(r||\"object\"!==x(e))i(n,e);else for(t in e)Pt(n+\"[\"+t+\"]\",e[t],r,i)}ce.param=function(e,t){var n,r=[],i=function(e,t){var n=v(t)?t():t;r[r.length]=encodeURIComponent(e)+\"=\"+encodeURIComponent(null==n?\"\":n)};if(null==e)return\"\";if(Array.isArray(e)||e.jquery&&!ce.isPlainObject(e))ce.each(e,function(){i(this.name,this.value)});else for(n in e)Pt(n,e[n],t,i);return r.join(\"&\")},ce.fn.extend({serialize:function(){return ce.param(this.serializeArray())},serializeArray:function(){return this.map(function(){var e=ce.prop(this,\"elements\");return e?ce.makeArray(e):this}).filter(function(){var e=this.type;return this.name&&!ce(this).is(\":disabled\")&&Ot.test(this.nodeName)&&!Ht.test(e)&&(this.checked||!we.test(e))}).map(function(e,t){var n=ce(this).val();return null==n?null:Array.isArray(n)?ce.map(n,function(e){return{name:t.name,value:e.replace(Lt,\"\\r\\n\")}}):{name:t.name,value:n.replace(Lt,\"\\r\\n\")}}).get()}});var Mt=/%20/g,Rt=/#.*$/,It=/([?&])_=[^&]*/,Wt=/^(.*?):[ \\t]*([^\\r\\n]*)$/gm,Ft=/^(?:GET|HEAD)$/,$t=/^\\/\\//,Bt={},_t={},zt=\"*/\".concat(\"*\"),Xt=C.createElement(\"a\");function Ut(o){return function(e,t){\"string\"!=typeof e&&(t=e,e=\"*\");var n,r=0,i=e.toLowerCase().match(D)||[];if(v(t))while(n=i[r++])\"+\"===n[0]?(n=n.slice(1)||\"*\",(o[n]=o[n]||[]).unshift(t)):(o[n]=o[n]||[]).push(t)}}function Vt(t,i,o,a){var s={},u=t===_t;function l(e){var r;return s[e]=!0,ce.each(t[e]||[],function(e,t){var n=t(i,o,a);return\"string\"!=typeof n||u||s[n]?u?!(r=n):void 0:(i.dataTypes.unshift(n),l(n),!1)}),r}return l(i.dataTypes[0])||!s[\"*\"]&&l(\"*\")}function Gt(e,t){var n,r,i=ce.ajaxSettings.flatOptions||{};for(n in t)void 0!==t[n]&&((i[n]?e:r||(r={}))[n]=t[n]);return r&&ce.extend(!0,e,r),e}Xt.href=Et.href,ce.extend({active:0,lastModified:{},etag:{},ajaxSettings:{url:Et.href,type:\"GET\",isLocal:/^(?:about|app|app-storage|.+-extension|file|res|widget):$/.test(Et.protocol),global:!0,processData:!0,async:!0,contentType:\"application/x-www-form-urlencoded; charset=UTF-8\",accepts:{\"*\":zt,text:\"text/plain\",html:\"text/html\",xml:\"application/xml, text/xml\",json:\"application/json, text/javascript\"},contents:{xml:/\\bxml\\b/,html:/\\bhtml/,json:/\\bjson\\b/},responseFields:{xml:\"responseXML\",text:\"responseText\",json:\"responseJSON\"},converters:{\"* text\":String,\"text html\":!0,\"text json\":JSON.parse,\"text xml\":ce.parseXML},flatOptions:{url:!0,context:!0}},ajaxSetup:function(e,t){return t?Gt(Gt(e,ce.ajaxSettings),t):Gt(ce.ajaxSettings,e)},ajaxPrefilter:Ut(Bt),ajaxTransport:Ut(_t),ajax:function(e,t){\"object\"==typeof e&&(t=e,e=void 0),t=t||{};var c,f,p,n,d,r,h,g,i,o,v=ce.ajaxSetup({},t),y=v.context||v,m=v.context&&(y.nodeType||y.jquery)?ce(y):ce.event,x=ce.Deferred(),b=ce.Callbacks(\"once memory\"),w=v.statusCode||{},a={},s={},u=\"canceled\",T={readyState:0,getResponseHeader:function(e){var t;if(h){if(!n){n={};while(t=Wt.exec(p))n[t[1].toLowerCase()+\" \"]=(n[t[1].toLowerCase()+\" \"]||[]).concat(t[2])}t=n[e.toLowerCase()+\" \"]}return null==t?null:t.join(\", \")},getAllResponseHeaders:function(){return h?p:null},setRequestHeader:function(e,t){return null==h&&(e=s[e.toLowerCase()]=s[e.toLowerCase()]||e,a[e]=t),this},overrideMimeType:function(e){return null==h&&(v.mimeType=e),this},statusCode:function(e){var t;if(e)if(h)T.always(e[T.status]);else for(t in e)w[t]=[w[t],e[t]];return this},abort:function(e){var t=e||u;return c&&c.abort(t),l(0,t),this}};if(x.promise(T),v.url=((e||v.url||Et.href)+\"\").replace($t,Et.protocol+\"//\"),v.type=t.method||t.type||v.method||v.type,v.dataTypes=(v.dataType||\"*\").toLowerCase().match(D)||[\"\"],null==v.crossDomain){r=C.createElement(\"a\");try{r.href=v.url,r.href=r.href,v.crossDomain=Xt.protocol+\"//\"+Xt.host!=r.protocol+\"//\"+r.host}catch(e){v.crossDomain=!0}}if(v.data&&v.processData&&\"string\"!=typeof v.data&&(v.data=ce.param(v.data,v.traditional)),Vt(Bt,v,t,T),h)return T;for(i in(g=ce.event&&v.global)&&0==ce.active++&&ce.event.trigger(\"ajaxStart\"),v.type=v.type.toUpperCase(),v.hasContent=!Ft.test(v.type),f=v.url.replace(Rt,\"\"),v.hasContent?v.data&&v.processData&&0===(v.contentType||\"\").indexOf(\"application/x-www-form-urlencoded\")&&(v.data=v.data.replace(Mt,\"+\")):(o=v.url.slice(f.length),v.data&&(v.processData||\"string\"==typeof v.data)&&(f+=(At.test(f)?\"&\":\"?\")+v.data,delete v.data),!1===v.cache&&(f=f.replace(It,\"$1\"),o=(At.test(f)?\"&\":\"?\")+\"_=\"+jt.guid+++o),v.url=f+o),v.ifModified&&(ce.lastModified[f]&&T.setRequestHeader(\"If-Modified-Since\",ce.lastModified[f]),ce.etag[f]&&T.setRequestHeader(\"If-None-Match\",ce.etag[f])),(v.data&&v.hasContent&&!1!==v.contentType||t.contentType)&&T.setRequestHeader(\"Content-Type\",v.contentType),T.setRequestHeader(\"Accept\",v.dataTypes[0]&&v.accepts[v.dataTypes[0]]?v.accepts[v.dataTypes[0]]+(\"*\"!==v.dataTypes[0]?\", \"+zt+\"; q=0.01\":\"\"):v.accepts[\"*\"]),v.headers)T.setRequestHeader(i,v.headers[i]);if(v.beforeSend&&(!1===v.beforeSend.call(y,T,v)||h))return T.abort();if(u=\"abort\",b.add(v.complete),T.done(v.success),T.fail(v.error),c=Vt(_t,v,t,T)){if(T.readyState=1,g&&m.trigger(\"ajaxSend\",[T,v]),h)return T;v.async&&0<v.timeout&&(d=ie.setTimeout(function(){T.abort(\"timeout\")},v.timeout));try{h=!1,c.send(a,l)}catch(e){if(h)throw e;l(-1,e)}}else l(-1,\"No Transport\");function l(e,t,n,r){var i,o,a,s,u,l=t;h||(h=!0,d&&ie.clearTimeout(d),c=void 0,p=r||\"\",T.readyState=0<e?4:0,i=200<=e&&e<300||304===e,n&&(s=function(e,t,n){var r,i,o,a,s=e.contents,u=e.dataTypes;while(\"*\"===u[0])u.shift(),void 0===r&&(r=e.mimeType||t.getResponseHeader(\"Content-Type\"));if(r)for(i in s)if(s[i]&&s[i].test(r)){u.unshift(i);break}if(u[0]in n)o=u[0];else{for(i in n){if(!u[0]||e.converters[i+\" \"+u[0]]){o=i;break}a||(a=i)}o=o||a}if(o)return o!==u[0]&&u.unshift(o),n[o]}(v,T,n)),!i&&-1<ce.inArray(\"script\",v.dataTypes)&&ce.inArray(\"json\",v.dataTypes)<0&&(v.converters[\"text script\"]=function(){}),s=function(e,t,n,r){var i,o,a,s,u,l={},c=e.dataTypes.slice();if(c[1])for(a in e.converters)l[a.toLowerCase()]=e.converters[a];o=c.shift();while(o)if(e.responseFields[o]&&(n[e.responseFields[o]]=t),!u&&r&&e.dataFilter&&(t=e.dataFilter(t,e.dataType)),u=o,o=c.shift())if(\"*\"===o)o=u;else if(\"*\"!==u&&u!==o){if(!(a=l[u+\" \"+o]||l[\"* \"+o]))for(i in l)if((s=i.split(\" \"))[1]===o&&(a=l[u+\" \"+s[0]]||l[\"* \"+s[0]])){!0===a?a=l[i]:!0!==l[i]&&(o=s[0],c.unshift(s[1]));break}if(!0!==a)if(a&&e[\"throws\"])t=a(t);else try{t=a(t)}catch(e){return{state:\"parsererror\",error:a?e:\"No conversion from \"+u+\" to \"+o}}}return{state:\"success\",data:t}}(v,s,T,i),i?(v.ifModified&&((u=T.getResponseHeader(\"Last-Modified\"))&&(ce.lastModified[f]=u),(u=T.getResponseHeader(\"etag\"))&&(ce.etag[f]=u)),204===e||\"HEAD\"===v.type?l=\"nocontent\":304===e?l=\"notmodified\":(l=s.state,o=s.data,i=!(a=s.error))):(a=l,!e&&l||(l=\"error\",e<0&&(e=0))),T.status=e,T.statusText=(t||l)+\"\",i?x.resolveWith(y,[o,l,T]):x.rejectWith(y,[T,l,a]),T.statusCode(w),w=void 0,g&&m.trigger(i?\"ajaxSuccess\":\"ajaxError\",[T,v,i?o:a]),b.fireWith(y,[T,l]),g&&(m.trigger(\"ajaxComplete\",[T,v]),--ce.active||ce.event.trigger(\"ajaxStop\")))}return T},getJSON:function(e,t,n){return ce.get(e,t,n,\"json\")},getScript:function(e,t){return ce.get(e,void 0,t,\"script\")}}),ce.each([\"get\",\"post\"],function(e,i){ce[i]=function(e,t,n,r){return v(t)&&(r=r||n,n=t,t=void 0),ce.ajax(ce.extend({url:e,type:i,dataType:r,data:t,success:n},ce.isPlainObject(e)&&e))}}),ce.ajaxPrefilter(function(e){var t;for(t in e.headers)\"content-type\"===t.toLowerCase()&&(e.contentType=e.headers[t]||\"\")}),ce._evalUrl=function(e,t,n){return ce.ajax({url:e,type:\"GET\",dataType:\"script\",cache:!0,async:!1,global:!1,converters:{\"text script\":function(){}},dataFilter:function(e){ce.globalEval(e,t,n)}})},ce.fn.extend({wrapAll:function(e){var t;return this[0]&&(v(e)&&(e=e.call(this[0])),t=ce(e,this[0].ownerDocument).eq(0).clone(!0),this[0].parentNode&&t.insertBefore(this[0]),t.map(function(){var e=this;while(e.firstElementChild)e=e.firstElementChild;return e}).append(this)),this},wrapInner:function(n){return v(n)?this.each(function(e){ce(this).wrapInner(n.call(this,e))}):this.each(function(){var e=ce(this),t=e.contents();t.length?t.wrapAll(n):e.append(n)})},wrap:function(t){var n=v(t);return this.each(function(e){ce(this).wrapAll(n?t.call(this,e):t)})},unwrap:function(e){return this.parent(e).not(\"body\").each(function(){ce(this).replaceWith(this.childNodes)}),this}}),ce.expr.pseudos.hidden=function(e){return!ce.expr.pseudos.visible(e)},ce.expr.pseudos.visible=function(e){return!!(e.offsetWidth||e.offsetHeight||e.getClientRects().length)},ce.ajaxSettings.xhr=function(){try{return new ie.XMLHttpRequest}catch(e){}};var Yt={0:200,1223:204},Qt=ce.ajaxSettings.xhr();le.cors=!!Qt&&\"withCredentials\"in Qt,le.ajax=Qt=!!Qt,ce.ajaxTransport(function(i){var o,a;if(le.cors||Qt&&!i.crossDomain)return{send:function(e,t){var n,r=i.xhr();if(r.open(i.type,i.url,i.async,i.username,i.password),i.xhrFields)for(n in i.xhrFields)r[n]=i.xhrFields[n];for(n in i.mimeType&&r.overrideMimeType&&r.overrideMimeType(i.mimeType),i.crossDomain||e[\"X-Requested-With\"]||(e[\"X-Requested-With\"]=\"XMLHttpRequest\"),e)r.setRequestHeader(n,e[n]);o=function(e){return function(){o&&(o=a=r.onload=r.onerror=r.onabort=r.ontimeout=r.onreadystatechange=null,\"abort\"===e?r.abort():\"error\"===e?\"number\"!=typeof r.status?t(0,\"error\"):t(r.status,r.statusText):t(Yt[r.status]||r.status,r.statusText,\"text\"!==(r.responseType||\"text\")||\"string\"!=typeof r.responseText?{binary:r.response}:{text:r.responseText},r.getAllResponseHeaders()))}},r.onload=o(),a=r.onerror=r.ontimeout=o(\"error\"),void 0!==r.onabort?r.onabort=a:r.onreadystatechange=function(){4===r.readyState&&ie.setTimeout(function(){o&&a()})},o=o(\"abort\");try{r.send(i.hasContent&&i.data||null)}catch(e){if(o)throw e}},abort:function(){o&&o()}}}),ce.ajaxPrefilter(function(e){e.crossDomain&&(e.contents.script=!1)}),ce.ajaxSetup({accepts:{script:\"text/javascript, application/javascript, application/ecmascript, application/x-ecmascript\"},contents:{script:/\\b(?:java|ecma)script\\b/},converters:{\"text script\":function(e){return ce.globalEval(e),e}}}),ce.ajaxPrefilter(\"script\",function(e){void 0===e.cache&&(e.cache=!1),e.crossDomain&&(e.type=\"GET\")}),ce.ajaxTransport(\"script\",function(n){var r,i;if(n.crossDomain||n.scriptAttrs)return{send:function(e,t){r=ce(\"<script>\").attr(n.scriptAttrs||{}).prop({charset:n.scriptCharset,src:n.url}).on(\"load error\",i=function(e){r.remove(),i=null,e&&t(\"error\"===e.type?404:200,e.type)}),C.head.appendChild(r[0])},abort:function(){i&&i()}}});var Jt,Kt=[],Zt=/(=)\\?(?=&|$)|\\?\\?/;ce.ajaxSetup({jsonp:\"callback\",jsonpCallback:function(){var e=Kt.pop()||ce.expando+\"_\"+jt.guid++;return this[e]=!0,e}}),ce.ajaxPrefilter(\"json jsonp\",function(e,t,n){var r,i,o,a=!1!==e.jsonp&&(Zt.test(e.url)?\"url\":\"string\"==typeof e.data&&0===(e.contentType||\"\").indexOf(\"application/x-www-form-urlencoded\")&&Zt.test(e.data)&&\"data\");if(a||\"jsonp\"===e.dataTypes[0])return r=e.jsonpCallback=v(e.jsonpCallback)?e.jsonpCallback():e.jsonpCallback,a?e[a]=e[a].replace(Zt,\"$1\"+r):!1!==e.jsonp&&(e.url+=(At.test(e.url)?\"&\":\"?\")+e.jsonp+\"=\"+r),e.converters[\"script json\"]=function(){return o||ce.error(r+\" was not called\"),o[0]},e.dataTypes[0]=\"json\",i=ie[r],ie[r]=function(){o=arguments},n.always(function(){void 0===i?ce(ie).removeProp(r):ie[r]=i,e[r]&&(e.jsonpCallback=t.jsonpCallback,Kt.push(r)),o&&v(i)&&i(o[0]),o=i=void 0}),\"script\"}),le.createHTMLDocument=((Jt=C.implementation.createHTMLDocument(\"\").body).innerHTML=\"<form></form><form></form>\",2===Jt.childNodes.length),ce.parseHTML=function(e,t,n){return\"string\"!=typeof e?[]:(\"boolean\"==typeof t&&(n=t,t=!1),t||(le.createHTMLDocument?((r=(t=C.implementation.createHTMLDocument(\"\")).createElement(\"base\")).href=C.location.href,t.head.appendChild(r)):t=C),o=!n&&[],(i=w.exec(e))?[t.createElement(i[1])]:(i=Ae([e],t,o),o&&o.length&&ce(o).remove(),ce.merge([],i.childNodes)));var r,i,o},ce.fn.load=function(e,t,n){var r,i,o,a=this,s=e.indexOf(\" \");return-1<s&&(r=Tt(e.slice(s)),e=e.slice(0,s)),v(t)?(n=t,t=void 0):t&&\"object\"==typeof t&&(i=\"POST\"),0<a.length&&ce.ajax({url:e,type:i||\"GET\",dataType:\"html\",data:t}).done(function(e){o=arguments,a.html(r?ce(\"<div>\").append(ce.parseHTML(e)).find(r):e)}).always(n&&function(e,t){a.each(function(){n.apply(this,o||[e.responseText,t,e])})}),this},ce.expr.pseudos.animated=function(t){return ce.grep(ce.timers,function(e){return t===e.elem}).length},ce.offset={setOffset:function(e,t,n){var r,i,o,a,s,u,l=ce.css(e,\"position\"),c=ce(e),f={};\"static\"===l&&(e.style.position=\"relative\"),s=c.offset(),o=ce.css(e,\"top\"),u=ce.css(e,\"left\"),(\"absolute\"===l||\"fixed\"===l)&&-1<(o+u).indexOf(\"auto\")?(a=(r=c.position()).top,i=r.left):(a=parseFloat(o)||0,i=parseFloat(u)||0),v(t)&&(t=t.call(e,n,ce.extend({},s))),null!=t.top&&(f.top=t.top-s.top+a),null!=t.left&&(f.left=t.left-s.left+i),\"using\"in t?t.using.call(e,f):c.css(f)}},ce.fn.extend({offset:function(t){if(arguments.length)return void 0===t?this:this.each(function(e){ce.offset.setOffset(this,t,e)});var e,n,r=this[0];return r?r.getClientRects().length?(e=r.getBoundingClientRect(),n=r.ownerDocument.defaultView,{top:e.top+n.pageYOffset,left:e.left+n.pageXOffset}):{top:0,left:0}:void 0},position:function(){if(this[0]){var e,t,n,r=this[0],i={top:0,left:0};if(\"fixed\"===ce.css(r,\"position\"))t=r.getBoundingClientRect();else{t=this.offset(),n=r.ownerDocument,e=r.offsetParent||n.documentElement;while(e&&(e===n.body||e===n.documentElement)&&\"static\"===ce.css(e,\"position\"))e=e.parentNode;e&&e!==r&&1===e.nodeType&&((i=ce(e).offset()).top+=ce.css(e,\"borderTopWidth\",!0),i.left+=ce.css(e,\"borderLeftWidth\",!0))}return{top:t.top-i.top-ce.css(r,\"marginTop\",!0),left:t.left-i.left-ce.css(r,\"marginLeft\",!0)}}},offsetParent:function(){return this.map(function(){var e=this.offsetParent;while(e&&\"static\"===ce.css(e,\"position\"))e=e.offsetParent;return e||J})}}),ce.each({scrollLeft:\"pageXOffset\",scrollTop:\"pageYOffset\"},function(t,i){var o=\"pageYOffset\"===i;ce.fn[t]=function(e){return M(this,function(e,t,n){var r;if(y(e)?r=e:9===e.nodeType&&(r=e.defaultView),void 0===n)return r?r[i]:e[t];r?r.scrollTo(o?r.pageXOffset:n,o?n:r.pageYOffset):e[t]=n},t,e,arguments.length)}}),ce.each([\"top\",\"left\"],function(e,n){ce.cssHooks[n]=Ye(le.pixelPosition,function(e,t){if(t)return t=Ge(e,n),_e.test(t)?ce(e).position()[n]+\"px\":t})}),ce.each({Height:\"height\",Width:\"width\"},function(a,s){ce.each({padding:\"inner\"+a,content:s,\"\":\"outer\"+a},function(r,o){ce.fn[o]=function(e,t){var n=arguments.length&&(r||\"boolean\"!=typeof e),i=r||(!0===e||!0===t?\"margin\":\"border\");return M(this,function(e,t,n){var r;return y(e)?0===o.indexOf(\"outer\")?e[\"inner\"+a]:e.document.documentElement[\"client\"+a]:9===e.nodeType?(r=e.documentElement,Math.max(e.body[\"scroll\"+a],r[\"scroll\"+a],e.body[\"offset\"+a],r[\"offset\"+a],r[\"client\"+a])):void 0===n?ce.css(e,t,i):ce.style(e,t,n,i)},s,n?e:void 0,n)}})}),ce.each([\"ajaxStart\",\"ajaxStop\",\"ajaxComplete\",\"ajaxError\",\"ajaxSuccess\",\"ajaxSend\"],function(e,t){ce.fn[t]=function(e){return this.on(t,e)}}),ce.fn.extend({bind:function(e,t,n){return this.on(e,null,t,n)},unbind:function(e,t){return this.off(e,null,t)},delegate:function(e,t,n,r){return this.on(t,e,n,r)},undelegate:function(e,t,n){return 1===arguments.length?this.off(e,\"**\"):this.off(t,e||\"**\",n)},hover:function(e,t){return this.on(\"mouseenter\",e).on(\"mouseleave\",t||e)}}),ce.each(\"blur focus focusin focusout resize scroll click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup contextmenu\".split(\" \"),function(e,n){ce.fn[n]=function(e,t){return 0<arguments.length?this.on(n,null,e,t):this.trigger(n)}});var en=/^[\\s\\uFEFF\\xA0]+|([^\\s\\uFEFF\\xA0])[\\s\\uFEFF\\xA0]+$/g;ce.proxy=function(e,t){var n,r,i;if(\"string\"==typeof t&&(n=e[t],t=e,e=n),v(e))return r=ae.call(arguments,2),(i=function(){return e.apply(t||this,r.concat(ae.call(arguments)))}).guid=e.guid=e.guid||ce.guid++,i},ce.holdReady=function(e){e?ce.readyWait++:ce.ready(!0)},ce.isArray=Array.isArray,ce.parseJSON=JSON.parse,ce.nodeName=fe,ce.isFunction=v,ce.isWindow=y,ce.camelCase=F,ce.type=x,ce.now=Date.now,ce.isNumeric=function(e){var t=ce.type(e);return(\"number\"===t||\"string\"===t)&&!isNaN(e-parseFloat(e))},ce.trim=function(e){return null==e?\"\":(e+\"\").replace(en,\"$1\")},\"function\"==typeof define&&define.amd&&define(\"jquery\",[],function(){return ce});var tn=ie.jQuery,nn=ie.$;return ce.noConflict=function(e){return ie.$===ce&&(ie.$=nn),e&&ie.jQuery===ce&&(ie.jQuery=tn),ce},\"undefined\"==typeof e&&(ie.jQuery=ie.$=ce),ce});\n";
	server.send(200, "text/css", css); // send to someones browser when asked
}

void handle_dashboard()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	webString = "<script type=\"text/javascript\">\n";
	webString += "function reloadSysInfo() {\n";
	webString += "$(\"#sysInfo\").load(\"/sysinfo\", function () { setTimeout(reloadSysInfo, 60000) });\n";
	webString += "}\n";
	webString += "setTimeout(reloadSysInfo(), 100);\n";
	webString += "function reloadSidebarInfo() {\n";
	webString += "$(\"#sidebarInfo\").load(\"/sidebarInfo\", function () { setTimeout(reloadSidebarInfo, 10000) });\n";
	webString += "}\n";
	webString += "setTimeout(reloadSidebarInfo, 200);\n";
	webString += "function reloadlastHeard() {\n";
	webString += "$(\"#lastHeard\").load(\"/lastHeard\", function () { setTimeout(reloadlastHeard, 10000) });\n";
	webString += "}\n";
	webString += "setTimeout(reloadlastHeard, 300);\n";
	webString += "$(window).trigger('resize');\n";
	webString += "</script>\n";

	webString += "<div id=\"sysInfo\">\n";
	webString += "</div>\n";

	webString += "<br />\n";
	webString += "<div class=\"nav-status\">\n";
	webString += "<div id=\"sidebarInfo\">\n";
	webString += "</div>\n";
	webString += "<br />\n";

	webString += "<table>\n";
	webString += "<tr>\n";
	webString += "<th colspan=\"2\">Radio Info</th>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>Freq TX</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(config.freq_tx, 4) + " MHz</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>Freq RX</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(config.freq_rx, 4) + " MHz</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>H/L</td>\n";
	if (config.rf_power)
		webString += "<td>HIGH</td>\n";
	else
		webString += "<td>LOW</td>\n";
	webString += "</tr>\n";
	webString += "</table>\n";
	webString += "\n";
	webString += "<br />\n";
	webString += "<table>\n";
	webString += "<tr>\n";
	webString += "<th colspan=\"2\">APRS SERVER</th>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>HOST</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(config.aprs_host) + "</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>PORT</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(config.aprs_port) + "</td>\n";
	webString += "</tr>\n";
	webString += "</table>\n";
	webString += "<br />\n";
	webString += "<table>\n";
	webString += "<tr>\n";
	webString += "<th colspan=\"2\">WiFi</th>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>MODE</td>\n";
	String strWiFiMode = "OFF";
	if (config.wifi_mode == WIFI_STA)
	{
		strWiFiMode = "STA";
	}
	else if (config.wifi_mode == WIFI_AP)
	{
		strWiFiMode = "AP";
	}
	else if (config.wifi_mode == WIFI_AP_STA)
	{
		strWiFiMode = "AP+STA";
	}
	webString += "<td style=\"background: #ffffff;\">" + strWiFiMode + "</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>SSID</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(WiFi.SSID()) + "</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<td>RSSI</td>\n";
	if (WiFi.isConnected())
		webString += "<td style=\"background: #ffffff;\">" + String(WiFi.RSSI()) + " dBm</td>\n";
	else
		webString += "<td style=\"background:#606060; color:#b0b0b0;\" aria-disabled=\"true>\">Disconnect</td>\n";
	webString += "</tr>\n";
	webString += "</table>\n";
	webString += "<br />\n";
	webString += "<table>\n";
	webString += "<tr>\n";
	webString += "<th colspan=\"2\">Bluetooth</th>\n";
	webString += "</tr>\n";
	webString += "<td>Master</td>\n";
	if (config.bt_master)
		webString += "<td style=\"background:#0b0; color:#030; width:50%;\">Enabled</td>\n";
	else
		webString += "<td style=\"background:#606060; color:#b0b0b0;\" aria-disabled=\"true>\">Disabled</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<tr>\n";
	webString += "<td>NAME</td>\n";
	webString += "<td style=\"background: #ffffff;\">" + String(config.bt_name) + "</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "<tr>\n";
	webString += "<td>MODE</td>\n";
	String btMode = "";
	if (config.bt_mode == 1)
	{
		btMode = "TNC2";
	}
	else if (config.bt_mode == 2)
	{
		btMode = "KISS";
	}
	else
	{
		btMode = "NONE";
	}
	webString += "<td style=\"background: #ffffff;\">" + btMode + "</td>\n";
	webString += "</tr>\n";
	webString += "<tr>\n";
	webString += "</table>\n";
	webString += "</div>\n";

	webString += "</div>\n";
	webString += "\n";
	webString += "<div class=\"content\">\n";
	// webString += "<b>LAST HEARD</b>\n";
	webString += "<div id=\"lastHeard\">\n";
	webString += "</div>\n";

	server.send(200, "text/html", webString); // send to someones browser when asked
	delay(100);
	webString.clear();
}

void handle_sidebar()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	String html = "<table style=\"background:white;border-collapse: unset;\">\n";
	html += "<tr>\n";
	html += "<th colspan=\"2\">Modes Enabled</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	if (config.igate_en)
		html += "<th style=\"background:#0b0; color:#030; width:50%;border-radius: 10px;border: 2px solid white;\">IGATE</th>\n";
	else
		html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\">IGATE</th>\n";

	if (config.digi_en)
		html += "<th style=\"background:#0b0; color:#030; width:50%;border-radius: 10px;border: 2px solid white;\">DIGI</th>\n";
	else
		html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\">DIGI</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	if (config.trk_en)
		html += "<th style=\"background:#0b0; color:#030; width:50%;border-radius: 10px;border: 2px solid white;\">TRACKER</th>\n";
	else
		html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\">TRACKER</th>\n";
	html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\">SAT</th>\n";
	html += "</tr>\n";
	html += "</table>\n";
	html += "<br />\n";
	html += "<table style=\"background:white;border-collapse: unset;\">\n";
	html += "<tr>\n";
	html += "<th colspan=\"2\">Network Status</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	if (aprsClient.connected() == true)
		html += "<th style=\"background:#0b0; color:#030; width:50%;border-radius: 10px;border: 2px solid white;\">APRS-IS</th>\n";
	else
		html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\" aria-disabled=\"true>\">APRS-IS</th>\n";
	if (wireguard_active() == true)
		html += "<th style=\"background:#0b0; color:#030; width:50%;border-radius: 10px;border: 2px solid white;\">VPN</th>\n";
	else
		html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\" aria-disabled=\"true>\">VPN</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\" aria-disabled=\"true>\">4G LTE</th>\n";
	html += "<th style=\"background:#606060; color:#b0b0b0;border-radius: 10px;border: 2px solid white;\" aria-disabled=\"true>\">MQTT</th>\n";
	html += "</tr>\n";
	html += "</table>\n";
	html += "<br />\n";
	html += "<table>\n";
	html += "<tr>\n";
	html += "<th colspan=\"2\">STATISTICS</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">PACKET RX:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.rxCount) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">PACKET TX:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.txCount) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">RF2INET:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.rf2inet) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">INET2RF:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.inet2rf) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">DIGI:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.digiCount) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td style=\"width: 60px;text-align: right;\">DROP/ERR:</td>\n";
	html += "<td style=\"background: #ffffff;\">" + String(status.dropCount) + "/" + String(status.errorCount) + "</td>\n";
	html += "</tr>\n";
	html += "</table>\n";
	html += "<br />\n";
	html += "<table>\n";
	html += "<tr>\n";
	html += "<th colspan=\"2\">GPS Info</th>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td>LAT:</td>\n";
	html += "<td style=\"background: #ffffff;text-align: left;\">" + String(gps.location.lat(), 5) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td>LON:</td>\n";
	html += "<td style=\"background: #ffffff;text-align: left;\">" + String(gps.location.lng(), 5) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td>ALT:</td>\n";
	html += "<td style=\"background: #ffffff;text-align: left;\">" + String(gps.altitude.meters(), 1) + "</td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td>SAT:</td>\n";
	html += "<td style=\"background: #ffffff;text-align: left;\">" + String(gps.satellites.value()) + "</td>\n";
	html += "</tr>\n";
	html += "</table>\n";

	html += "<script>\n";
	html += "$(window).trigger('resize');\n";
	html += "</script>\n";
	server.send(200, "text/html", html); // send to someones browser when asked
	delay(100);
	html.clear();
}

void handle_symbol()
{
	int i;
	String html = "<table border=\"1\" align=\"center\">\n";
	html += "<tr><th colspan=\"16\">Table '/'</th></tr>\n";
	html += "<tr>\n";
	for (i = 33; i < 129; i++)
	{
		html += "<td><img src=\"http://www.dprns.com/symbols/icons/" + String(i) + "-1.png\"></td>\n";
		if (((i % 16) == 0) && (i < 126))
			html += "</tr>\n<tr>\n";
	}
	html += "</tr>";
	html += "</table>\n<br />";
	html += "<table border=\"1\" align=\"center\">\n";
	html += "<tr><th colspan=\"16\">Table '\\'</th></tr>\n";
	html += "<tr>\n";
	for (i = 33; i < 129; i++)
	{
		html += "<td><img src=\"http://www.dprns.com/symbols/icons/" + String(i) + "-2.png\"></td>\n";
		if (((i % 16) == 0) && (i < 126))
			html += "</tr>\n<tr>\n";
	}
	html += "</tr>";
	html += "</table>\n";
	server.send(200, "text/html", html); // send to someones browser when asked
}

void handle_sysinfo()
{
	String html = "<table style=\"table-layout: fixed;border-collapse: unset;border-radius: 10px;border-color: #ee800a;border-style: ridge;border-spacing: 1px;border-width: 4px;background: #ee800a;\">\n";
	html += "<tr>\n";
	html += "<th><span><b>Up Time</b></span></th>\n";
	html += "<th><span>Free RAM(KByte)</span></th>\n";
	html += "<th><span>Free PSRAM(KByte)</span></th>\n";
	html += "<th><span>SD CARD(MByte)</span></th>\n";
	html += "<th><span>BAT(V)</span></th>\n";

	html += "</tr>\n";
	html += "<tr>\n";
	time_t tn = now() - systemUptime;
	// String uptime = String(day(tn) - 1, DEC) + "D " + String(hour(tn), DEC) + ":" + String(minute(tn), DEC) + ":" + String(second(tn), DEC);
	String uptime = String(day(tn) - 1, DEC) + "D " + String(hour(tn), DEC) + ":" + String(minute(tn), DEC);
	html += "<td><b>" + uptime + "</b></td>\n";
	html += "<td><b>" + String((float)ESP.getFreeHeap() / 1000, 1) + "/" + String((float)ESP.getHeapSize() / 1000, 1) + "</b></td>\n";
	html += "<td><b>" + String((float)ESP.getFreePsram() / 1000, 1) + "/" + String((float)ESP.getPsramSize() / 1000, 1) + "</b></td>\n";
	uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
	uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
	html += "<td><b>" + String(cardUsed) + "/" + String(cardTotal) + "</b></td>\n";
	html += "<td><b>" + String(vbat, 2) + "</b></td>\n";
	// html += "<td style=\"background: #f00\"><b>" + String(ESP.getCycleCount()) + "</b></td>\n";
	html += "</tr>\n";
	html += "</table>\n";
	server.send(200, "text/html", html); // send to someones browser when asked
	html.clear();
}

void handle_lastHeard()
{
	struct pbuf_t aprs;
	ParseAPRS aprsParse;
	struct tm tmstruct;
	String html = "";
	sort(pkgList, PKGLISTSIZE);

	html = "<table>\n";
	html += "<th colspan=\"7\" style=\"background-color: #070ac2;\">LAST HEARD</th>\n";
	html += "<tr>\n";
	html += "<th style=\"min-width:10ch\"><span><b>Time (";
	if (config.timeZone >= 0)
		html += "+";
	// else
	//	html += "-";

	if (config.timeZone == (int)config.timeZone)
		html += String((int)config.timeZone) + ")</b></span></th>\n";
	else
		html += String(config.timeZone, 1) + ")</b></span></th>\n";
	html += "<th style=\"min-width:16px\">ICON</th>\n";
	html += "<th style=\"min-width:10ch\">Callsign</th>\n";
	html += "<th>VIA LAST PATH</th>\n";
	html += "<th style=\"min-width:5ch\">DX</th>\n";
	html += "<th style=\"min-width:5ch\">PACKET</th>\n";
	html += "<th style=\"min-width:5ch\">AUDIO</th>\n";
	html += "</tr>\n";

	for (int i = 0; i < 30; i++)
	{
		pkgListType pkg = getPkgList(i);
		if (pkg.time > 0)
		{
			String line = String(pkg.raw);
			int packet = pkg.pkg;
			int start_val = line.indexOf(">", 0); // หาตำแหน่งแรกของ >
			if (start_val > 3)
			{
				String src_call = line.substring(0, start_val);
				memset(&aprs, 0, sizeof(pbuf_t));
				aprs.buf_len = 300;
				aprs.packet_len = line.length();
				line.toCharArray(&aprs.data[0], aprs.packet_len);
				int start_info = line.indexOf(":", 0);
				int end_ssid = line.indexOf(",", 0);
				int start_dst = line.indexOf(">", 2);
				int start_dstssid = line.indexOf("-", start_dst);
				String path = "";

				if ((end_ssid > start_dst) && (end_ssid < start_info))
				{
					path = line.substring(end_ssid + 1, start_info);
				}
				if (end_ssid < 5)
					end_ssid = start_info;
				if ((start_dstssid > start_dst) && (start_dstssid < start_dst + 10))
				{
					aprs.dstcall_end_or_ssid = &aprs.data[start_dstssid];
				}
				else
				{
					aprs.dstcall_end_or_ssid = &aprs.data[end_ssid];
				}
				aprs.info_start = &aprs.data[start_info + 1];
				aprs.dstname = &aprs.data[start_dst + 1];
				aprs.dstname_len = end_ssid - start_dst;
				aprs.dstcall_end = &aprs.data[end_ssid];
				aprs.srccall_end = &aprs.data[start_dst];

				// Serial.println(aprs.info_start);
				// aprsParse.parse_aprs(&aprs);
				if (aprsParse.parse_aprs(&aprs))
				{
					pkg.calsign[10] = 0;
					time_t tm = pkg.time;
					localtime_r(&pkg.time, &tmstruct);
					char strTime[10];
					sprintf(strTime, "%02d:%02d:%02d", tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
					// String str = String(tmstruct.tm_hour, DEC) + ":" + String(tmstruct.tm_min, DEC) + ":" + String(tmstruct.tm_sec, DEC);

					html += "<tr><td>" + String(strTime) + "</td>";
					String fileImg = "";
					uint8_t sym = (uint8_t)aprs.symbol[1];
					if (sym > 31 && sym < 127)
					{
						if (aprs.symbol[0] > 64 && aprs.symbol[0] < 91) // table A-Z
						{
							html += "<td><b>" + String(aprs.symbol[0]) + "</b></td>";
						}
						else
						{
							fileImg = String(sym, DEC);
							if (aprs.symbol[0] == 92)
							{
								fileImg += "-2.png";
							}
							else if (aprs.symbol[0] == 47)
							{
								fileImg += "-1.png";
							}
							else
							{
								fileImg = "dot.png";
							}
							html += "<td><img src=\"http://www.dprns.com/symbols/icons/" + fileImg + "\"></td>";
						}
					}
					else
					{
						html += "<td><img src=\"http://www.dprns.com/symbols/icons/dot.png\"></td>";
					}
					html += "<td>" + src_call;
					if (aprs.srcname_len > 0 && aprs.srcname_len < 10) // Get Item/Object
					{
						char itemname[10];
						memset(&itemname, 0, sizeof(itemname));
						memcpy(&itemname, aprs.srcname, aprs.srcname_len);
						html += "(" + String(itemname) + ")";
					}
					html += +"</td>";
					if (path == "")
					{
						html += "<td style=\"text-align: left;\">RF: DIRECT</td>";
					}
					else
					{
						String LPath = path.substring(path.lastIndexOf(',') + 1);
						// if(path.indexOf("qAR")>=0 || path.indexOf("qAS")>=0 || path.indexOf("qAC")>=0){ //Via from Internet Server
						if (path.indexOf("qA") >= 0 || path.indexOf("TCPIP") >= 0)
						{
							html += "<td style=\"text-align: left;\">INET: " + LPath + "</td>";
						}
						else
						{
							if (LPath.indexOf("*") > 0)
								html += "<td style=\"text-align: left;\">DIGI: " + path + "</td>";
							else
								html += "<td style=\"text-align: left;\">RF: " + path + "</td>";
						}
					}
					// html += "<td>" + path + "</td>";
					if (aprs.flags & F_HASPOS)
					{
						double lat, lon;
						if (gps.location.isValid())
						{
							lat = gps.location.lat();
							lon = gps.location.lng();
						}
						else
						{
							lat = config.igate_lat;
							lon = config.igate_lon;
						}
						double dtmp = aprsParse.direction(lon, lat, aprs.lng, aprs.lat);
						double dist = aprsParse.distance(lon, lat, aprs.lng, aprs.lat);
						html += "<td>" + String(dist, 1) + "km/" + String(dtmp, 0) + "°</td>";
					}
					else
					{
						html += "<td>-</td>\n";
					}
					html += "<td>" + String(packet) + "</td>\n";
					if (pkg.audio_level == 0)
					{
						html += "<td>-</td></tr>\n";
					}
					else
					{
						//Vp-p to dBV at http://earmark.net/gesr/opamp/db_calc.htm
						double Vrms = (double)pkg.audio_level / 1000;
						double audBV = 20.0F * log10(Vrms);
						if(audBV<-15.0F){ 
							html += "<td style=\"color: #0000f0;\">"; //Low wave amplitude <0.5Vp-p
						}else if(audBV>-4.0F){ 
							html += "<td style=\"color: #f00000;\">"; //High wave amplitude >1.8Vp-p
						}else{
							html += "<td style=\"color: #008000;\">";
						}
						html += String(audBV, 1) + "dBV</td></tr>\n";
					}
				}
			}
		}
	}
	html += "</table>\n";
	server.send(200, "text/html", html); // send to someones browser when asked
}

#ifdef SDCARD
void handle_storage()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	String dirname = "/";
	char strTime[100];

	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "SD_INIT")
			{
				// SD.end();
				// if (!SD.begin(SDCARD_CS, spiSD, SDSPEED)) {
				//	Serial.println("SD CARD Initialization failed!");
				//	//return;
				// }
			}
		}
	}

	//	setHTML(1);
	uint8_t cardType = SD.cardType();

	webString = "<table style=\"width:200px\">\n";
	webString += "<th colspan=\"2\"><span><b>SD Card information</b></span></th>\n";
	webString += "<tr>\n";
	webString += "<td align=\"right\"><b>SD CARD TYPE:</b></td><td>";
	if (cardType == CARD_NONE)
	{
		webString += "NOT FOUND\n";
	}
	else
	{
		if (cardType == CARD_MMC)
		{
			webString += "MMC\n";
		}
		else if (cardType == CARD_SD)
		{
			webString += "SDSC\n";
		}
		else if (cardType == CARD_SDHC)
		{
			webString += "SDHC\n";
		}
		else
		{
			webString += "UNKNOWN\n";
		}
		unsigned long cardSize = SD.cardSize() / (1024 * 1024);
		unsigned long cardTotal = SD.totalBytes() / (1024 * 1024);
		unsigned long cardUsed = SD.usedBytes() / (1024 * 1024);

		webString += "</td></tr>";
		webString += "<tr><td style=\"width: 60px;text-align: right;\"><b>SD Card Size: </b></td><td style=\"text-align: right;\">";
		webString += String((double)cardSize / 1000, 1) + "GB</td></tr>\n";
		webString += "<tr><td style=\"width: 60px;text-align: right;\"><b>Total space: </b></td><td style=\"text-align: right;\">";
		webString += String((unsigned long)cardTotal) + "MB</td></tr>\n";
		webString += "<tr><td style=\"width: 60px;text-align: right;\"><b>Used space: </b></td><td style=\"text-align: right;\">";
		webString += String((unsigned long)cardUsed) + "M</td></tr>\n";

		webString += "</table><br /><br /><h2>Listing directory: " + dirname + "</h2><br />\n";

		File root = SD.open(dirname);
		if (!root)
		{
			webString += "Failed to open directory\n";
			// return;
		}
		if (!root.isDirectory())
		{
			webString += "Not a directory";
			// return;
		}

		File file = root.openNextFile();
		webString += "<table border=\"1\"><tr align=\"center\" bgcolor=\"#03DDFC\"><td><b>DIRECTORY</b></td><td width=\"150\"><b>FILE NAME</b></td><td width=\"100\"><b>SIZE(Byte)</b></td><td width=\"170\"><b>DATE TIME</b></td><td><b>DEL</b></td></tr>";
		while (file)
		{
			if (file.isDirectory())
			{
				// webString += "<tr><td>DIR : ");
				webString += "<tr><td>" + String(file.name()) + "</td>";
				time_t t = file.getLastWrite();
				struct tm *tmstruct = localtime(&t);
				sprintf(strTime, "<td></td><td></td><td align=\"right\">%d-%02d-%02d %02d:%02d:%02d</td>", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
				webString += String(strTime);
				// if (levels) {
				//	listDir(fs, file.name(), levels - 1);
				// }
				webString += "<td></td></tr>\n";
			}
			else
			{
				/*Serial.print("  FILE: ");
				Serial.print(file.name());*/
				String fName = String(file.name()).substring(1);
				webString += "<tr><td>/</td><td align=\"right\"><a href=\"/download?FILE=" + fName + "\" target=\"_blank\">" + fName + "</a></td>";
				// Serial.print("  SIZE: ");
				webString += "<td align=\"right\">" + String(file.size()) + "</td>";
				time_t t = file.getLastWrite();
				struct tm *tmstruct = localtime(&t);
				sprintf(strTime, "<td align=\"right\">%d-%02d-%02d %02d:%02d:%02d</td>", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
				webString += String(strTime);
				webString += "<td align=\"center\"><a href=\"/delete?FILE=" + fName + "\">X</a></td></tr>\n";
			}
			file = root.openNextFile();
		}
		webString += "</table>";
	}

	// webString += "</br><div><a href=\"/file?SD_INIT=OK\">[SD INIT]</a></div> \n";
	server.send(200, "text/html", webString); // send to someones browser when asked
}

#endif

void handle_radio()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	// bool noiseEn=false;
	bool radioEnable = false;
	if (server.hasArg("commitRadio"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "radioEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						radioEnable = true;
					}
				}
			}

			if (server.argName(i) == "nw_band")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.band = server.arg(i).toInt();
						// if (server.arg(i).toInt())
						// 	config.band = 1;
						// else
						// 	config.band = 0;
					}
				}
			}

			if (server.argName(i) == "volume")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.volume = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "rf_power")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						if (server.arg(i).toInt())
							config.rf_power = true;
						else
							config.rf_power = false;
					}
				}
			}

			if (server.argName(i) == "sql_level")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.sql_level = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "tx_freq")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.freq_tx = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "rx_freq")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.freq_rx = server.arg(i).toFloat();
				}
			}

			if (server.argName(i) == "tx_offset")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.offset_tx = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "rx_offset")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.offset_rx = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "tx_ctcss")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.tone_tx = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "rx_ctcss")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.tone_rx = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "rf_type")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.rf_type = server.arg(i).toInt();
				}
			}
		}
		// config.noise=noiseEn;
		// config.agc=agcEn;
		config.rf_en = radioEnable;
		String html = "OK";
		server.send(200, "text/html", html); // send to someones browser when asked
		saveEEPROM();
		// delay(100);
		RF_MODULE(false);
	}
	else if (server.hasArg("commitTNC"))
	{
		bool hpf = 0;
		bool bpf = 0;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "HPF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						hpf = true;
					}
				}
			}
			if (server.argName(i) == "BPF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						bpf = true;
					}
				}
			}
			if (server.argName(i) == "timeSlot")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.tx_timeslot = server.arg(i).toInt();
					}
				}
			}
			if (server.argName(i) == "preamble")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.preamble = server.arg(i).toInt();
					}
				}
			}
		}
		config.audio_hpf = hpf;
		config.audio_bpf = bpf;
		afskSetHPF(config.audio_hpf);
		afskSetBPF(config.audio_bpf);
		String html = "OK";
		server.send(200, "text/html", html); // send to someones browser when asked
		saveEEPROM();
	}
	else
	{
		String html = "<script type=\"text/javascript\">\n";
		html += "var sliderVol = document.getElementById(\"sliderVolume\");\n";
		html += "var outputVol = document.getElementById(\"volShow\");\n";
		html += "var sliderSql = document.getElementById(\"sliderSql\");\n";
		html += "var outputSql = document.getElementById(\"sqlShow\");\n";
		html += "outputVol.innerHTML = sliderVol.value;\n";
		html += "outputSql.innerHTML = sliderSql.value;\n";
		html += "\n";
		html += "sliderVol.oninput = function () {\n";
		html += "outputVol.innerHTML = this.value;\n";
		html += "}\n";
		html += "sliderSql.oninput = function () {\n";
		html += "outputSql.innerHTML = this.value;\n";
		html += "}\n";
		html += "\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		html += "if(e.currentTarget.id===\"formRadio\") document.getElementById(\"submitRadio\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formTNC\") document.getElementById(\"submitTNC\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/radio',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "function rfType(){\n";
		html += "var type = document.getElementById(\"rf_type\").value;\n";
		html += "if(type==1||type==4||type==7){document.getElementById(\"tx_freq\").setAttribute(\"max\",174);document.getElementById(\"rx_freq\").setAttribute(\"max\",174);};\n";
		html += "if(type==1){document.getElementById(\"tx_freq\").setAttribute(\"min\",134);document.getElementById(\"rx_freq\").setAttribute(\"min\",134);};\n";
		html += "if(type==4||type==7){document.getElementById(\"tx_freq\").setAttribute(\"min\",136);document.getElementById(\"rx_freq\").setAttribute(\"min\",136);};\n";
		html += "if(type==2||type==5||type==8){document.getElementById(\"tx_freq\").setAttribute(\"max\",470);document.getElementById(\"rx_freq\").setAttribute(\"max\",470);};\n";
		html += "if(type==2||type==5||type==8){document.getElementById(\"tx_freq\").setAttribute(\"min\",400);document.getElementById(\"rx_freq\").setAttribute(\"min\",400);};\n";
		html += "if(type==3){document.getElementById(\"tx_freq\").setAttribute(\"min\",320);document.getElementById(\"rx_freq\").setAttribute(\"min\",320);};\n";
		html += "if(type==3){document.getElementById(\"tx_freq\").setAttribute(\"max\",400);document.getElementById(\"rx_freq\").setAttribute(\"max\",400);};\n";
		html += "if(type==6){document.getElementById(\"tx_freq\").setAttribute(\"min\",350);document.getElementById(\"rx_freq\").setAttribute(\"min\",350);};\n";
		html += "if(type==6){document.getElementById(\"tx_freq\").setAttribute(\"max\",390);document.getElementById(\"rx_freq\").setAttribute(\"max\",390);};\n";
		html += "if(type==1||type==4||type==7){document.getElementById(\"tx_freq\").setAttribute(\"value\",144.390);document.getElementById(\"rx_freq\").setAttribute(\"value\",144.390);};\n";
		html += "if(type==2||type==5||type==8){document.getElementById(\"tx_freq\").setAttribute(\"value\",432.5);document.getElementById(\"rx_freq\").setAttribute(\"value\",432.5);};\n";
		html += "\n";
		html += "}\n";
		html += "</script>\n";
		html += "<form id='formRadio' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>RF Analog Module</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Enable:</b></td>\n";
		String radioEnFlag = "";
		if (config.rf_en)
			radioEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"radioEnable\" value=\"OK\" " + radioEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Module Type:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"rf_type\" id=\"rf_type\" onchange=\"rfType()\">\n";
		for (int i = 0; i < 9; i++)
		{
			if (config.rf_type == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(RF_TYPE[i]) + "</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(RF_TYPE[i]) + "</option>\n";
		}
		html += "</select>\n";
		html += "</td>\n";
		float freqMin = 0;
		float freqMax = 0;
		switch (config.rf_type)
		{
		case RF_SA868_VHF:
			freqMin = 134.0F;
			freqMax = 174.0F;
			break;
		case RF_SR_1WV:
		case RF_SR_2WVS:
			freqMin = 136.0F;
			freqMax = 174.0F;
			break;
		case RF_SA868_350:
			freqMin = 320.0F;
			freqMax = 400.0F;
			break;
		case RF_SR_1W350:
			freqMin = 350.0F;
			freqMax = 390.0F;
			break;
		case RF_SA868_UHF:
		case RF_SR_1WU:
		case RF_SR_2WUS:
			freqMin = 400.0F;
			freqMax = 470.0F;
			break;
		default:
			freqMin = 134.0F;
			freqMax = 500.0F;
			break;
		}
		html += "<tr>\n";
		html += "<td align=\"right\"><b>TX Frequency:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input type=\"number\" id=\"tx_freq\" name=\"tx_freq\" min=\"" + String(freqMin, 4) + "\" max=\"" + String(freqMax, 4) + "\"\n";
		html += "step=\"0.0001\" value=\"" + String(config.freq_tx, 4) + "\" /> MHz</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>RX Frequency:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input type=\"number\" id=\"rx_freq\" name=\"rx_freq\" min=\"" + String(freqMin, 4) + "\" max=\"" + String(freqMax, 4) + "\"\n";
		html += "step=\"0.0001\" value=\"" + String(config.freq_rx, 4) + "\" /> Mhz</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>TX CTCSS:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"tx_ctcss\" id=\"tx_ctcss\">\n";
		for (int i = 0; i < 39; i++)
		{
			if (config.tone_tx == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(ctcss[i], 1) + "</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(ctcss[i], 1) + "</option>\n";
		}
		html += "</select> Hz\n";
		html += "</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>RX CTCSS:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"rx_ctcss\" id=\"rx_ctcss\">\n";
		html += "<option value=\"0\" selected>0.0</option>\n";
		for (int i = 0; i < 39; i++)
		{
			if (config.tone_rx == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(ctcss[i], 1) + "</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(ctcss[i], 1) + "</option>\n";
		}
		html += "</select> Hz\n";
		html += "</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Narrow/Wide:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"nw_band\" id=\"nw_band\">\n";
		String cmSelNWT = "";
		String cmSelNWF = "";
		if (config.band)
		{
			cmSelNWT = "selected";
		}
		else
		{
			cmSelNWF = "selected";
		}
		html += "<option value=\"1\" " + cmSelNWT + ">25.0KHz</option>\n";
		html += "<option value=\"0\" " + cmSelNWF + ">12.5KHz</option>\n";
		html += "</select>\n";
		html += "</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>TX Power:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"rf_power\" id=\"rf_power\">\n";
		String cmRfPwrF = "";
		String cmRfPwrT = "";
		if (config.rf_power)
		{
			cmRfPwrT = "selected";
		}
		else
		{
			cmRfPwrF = "selected";
		}
		html += "<option value=\"1\" " + cmRfPwrT + ">HIGH</option>\n";
		html += "<option value=\"0\" " + cmRfPwrF + ">LOW</option>\n";
		html += "</select>\n";
		html += "</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>VOLUME:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"sliderVolume\" name=\"volume\" type=\"range\"\n";
		html += "min=\"1\" max=\"8\" value=\"" + String(config.volume) + "\" /><b><span style=\"font-size: 14pt;\" id=\"volShow\">" + String(config.volume) + "</span></b></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>SQL Level:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"sliderSql\" name=\"sql_level\" type=\"range\"\n";
		html += "min=\"0\" max=\"8\" value=\"" + String(config.sql_level) + "\" /><b><span style=\"font-size: 14pt;\" id=\"sqlShow\">" + String(config.sql_level) + "</span></b></td>\n";
		html += "</tr>\n";
		html += "</table>\n";
		html += "<div class=\"form-group\">\n";
		html += "<label class=\"col-sm-4 col-xs-12 control-label\"></label>\n";
		html += "<div class=\"col-sm-2 col-xs-4\"><button type='submit' id='submitRadio' name=\"commitRadio\"> Apply Change </button></div>\n";
		html += "</div><br />\n";
		html += "<input type=\"hidden\" name=\"commitRadio\"/>\n";
		html += "</form>";

		// AFSK,TNC Configuration
		html += "<form id='formTNC' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>AFSK/TNC Configuration</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Audio HPF:</b></td>\n";
		String strFlag = "";
		if (config.audio_hpf)
			strFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"HPF\" value=\"OK\" " + strFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Audio high pass filter >1KHz cutoff 10Khz</i></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Audio BPF:</b></td>\n";
		strFlag = "";
		if (config.audio_bpf)
			strFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"BPF\" value=\"OK\" " + strFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Audio band pass filter 1Khz-2.5KHz</i></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>TX Time Slot:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input type=\"number\" name=\"timeSlot\" min=\"2000\" max=\"99999\"\n";
		html += "step=\"1000\" value=\"" + String(config.tx_timeslot) + "\" /> mSec.</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Preamble:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"preamble\">\n";
		for (int i = 1; i < 10; i++)
		{
			if (config.preamble == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(i * 100) + "</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(i * 100) + "</option>\n";
		}
		html += "</select> mSec.\n";
		html += "</td>\n";
		html += "</tr>\n";

		html += "</table>\n";
		html += "<div class=\"col-sm-2 col-xs-4\"><button type='submit' id='submitTNC'  name=\"commitTNC\"> Apply Change </button></div>\n";
		html += "<br />\n";
		html += "<input type=\"hidden\" name=\"commitTNC\"/>\n";
		html += "</form>";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

void handle_vpn()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	if (server.hasArg("commitVPN"))
	{
		bool vpnEn = false;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));

			if (server.argName(i) == "vpnEnable")
			{
				if (server.arg(i) != "")
				{
					// if (isValidNumber(server.arg(i)))
					if (String(server.arg(i)) == "OK")
						vpnEn = true;
				}
			}

			// if (server.argName(i) == "taretime") {
			//	if (server.arg(i) != "")
			//	{
			//		//if (isValidNumber(server.arg(i)))
			//		if (String(server.arg(i)) == "OK")
			//			taretime = true;
			//	}
			// }
			if (server.argName(i) == "wg_port")
			{
				if (server.arg(i) != "")
				{
					config.wg_port = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "wg_public_key")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_public_key, server.arg(i).c_str());
					config.wg_public_key[44] = 0;
				}
			}

			if (server.argName(i) == "wg_private_key")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_private_key, server.arg(i).c_str());
					config.wg_private_key[44] = 0;
				}
			}

			if (server.argName(i) == "wg_peer_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_peer_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_local_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_local_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_netmask_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_netmask_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_gw_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_gw_address, server.arg(i).c_str());
				}
			}
		}

		config.vpn = vpnEn;
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else
	{

		String html = "<script type=\"text/javascript\">\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		html += "if(e.currentTarget.id===\"formVPN\") document.getElementById(\"submitVPN\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/vpn',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "</script>\n";

		// html += "<h2>System Setting</h2>\n";
		html += "<form accept-charset=\"UTF-8\" action=\"#\" class=\"form-horizontal\" id=\"fromVPN\" method=\"post\">\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>Wireguard Configuration</b></span></th>\n";
		html += "<tr>";

		String syncFlage = "";
		if (config.vpn)
			syncFlage = "checked";
		html += "<td align=\"right\"><b>Enable</b></td>\n";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"vpnEnable\" value=\"OK\" " + syncFlage + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Server Address</b></td>\n";
		html += "<td style=\"text-align: left;\"><input  size=\"20\" id=\"wg_peer_address\" name=\"wg_peer_address\" type=\"text\" value=\"" + String(config.wg_peer_address) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Server Port</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"wg_port\" size=\"5\" name=\"wg_port\" type=\"number\" value=\"" + String(config.wg_port) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Local Address</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"wg_local_address\" name=\"wg_local_address\" type=\"text\" value=\"" + String(config.wg_local_address) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Netmask</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"wg_netmask_address\" name=\"wg_netmask_address\" type=\"text\" value=\"" + String(config.wg_netmask_address) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Gateway</b></td>\n";
		html += "<td style=\"text-align: left;\"><input id=\"wg_gw_address\" name=\"wg_gw_address\" type=\"text\" value=\"" + String(config.wg_gw_address) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Public Key</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"50\" maxlength=\"44\" id=\"wg_public_key\" name=\"wg_public_key\" type=\"text\" value=\"" + String(config.wg_public_key) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Private Key</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"50\" maxlength=\"44\" id=\"wg_private_key\" name=\"wg_private_key\" type=\"text\" value=\"" + String(config.wg_private_key) + "\" /></td>\n";
		html += "</tr>\n";

		html += "</table><br />\n";
		html += "<td><input class=\"btn btn-primary\" id=\"submitVPN\" name=\"commitVPN\" type=\"submit\" value=\"Save Config\" maxlength=\"80\"/></td>\n";
		html += "<input type=\"hidden\" name=\"commitVPN\"/>\n";
		html += "</form>\n";

		html += "</body></html>\n";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

void handle_system()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	if (server.hasArg("updateTimeZone"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "SetTimeZone")
			{
				if (server.arg(i) != "")
				{
					config.timeZone = server.arg(i).toFloat();
					// Serial.println("WEB Config Time Zone);
					configTime(3600 * config.timeZone, 0, config.ntp_host);
				}
				break;
			}
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("updateTimeNtp"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "SetTimeNtp")
			{
				if (server.arg(i) != "")
				{
					// Serial.println("WEB Config NTP");
					strcpy(config.ntp_host, server.arg(i).c_str());
					configTime(3600 * config.timeZone, 0, config.ntp_host);
				}
				break;
			}
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("updateTime"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "SetTime")
			{
				if (server.arg(i) != "")
				{
					// struct tm tmn;
					String date = getValue(server.arg(i), ' ', 0);
					String time = getValue(server.arg(i), ' ', 1);
					int yyyy = getValue(date, '-', 0).toInt();
					int mm = getValue(date, '-', 1).toInt();
					int dd = getValue(date, '-', 2).toInt();
					int hh = getValue(time, ':', 0).toInt();
					int ii = getValue(time, ':', 1).toInt();
					int ss = getValue(time, ':', 2).toInt();
					// int ss = 0;

					tmElements_t timeinfo;
					timeinfo.Year = yyyy - 1970;
					timeinfo.Month = mm;
					timeinfo.Day = dd;
					timeinfo.Hour = hh;
					timeinfo.Minute = ii;
					timeinfo.Second = ss;
					time_t timeStamp = makeTime(timeinfo);

					// tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec

					time_t rtc = timeStamp - (config.timeZone * 3600);
					timeval tv = {rtc, 0};
					timezone tz = {(0) + DST_MN, 0};
					settimeofday(&tv, &tz);

					// Serial.println("Update TIME " + server.arg(i));
					Serial.print("Set New Time at ");
					Serial.print(dd);
					Serial.print("/");
					Serial.print(mm);
					Serial.print("/");
					Serial.print(yyyy);
					Serial.print(" ");
					Serial.print(hh);
					Serial.print(":");
					Serial.print(ii);
					Serial.print(":");
					Serial.print(ss);
					Serial.print(" ");
					Serial.println(timeStamp);
				}
				break;
			}
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("REBOOT"))
	{
		esp_restart();
	}
	else if (server.hasArg("commitWebAuth"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "webauth_user")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.http_username, server.arg(i).c_str());
				}
				break;
			}
			if (server.argName(i) == "webauth_pass")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.http_password, server.arg(i).c_str());
				}
				break;
			}
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("commitDISP"))
	{
		bool dispRX = false;
		bool dispTX = false;
		bool dispRF = false;
		bool dispINET = false;
		bool oledEN = false;

		config.dispFilter = 0;

		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "oledEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						oledEN = true;
					}
				}
			}
			if (server.argName(i) == "filterMessage")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_MESSAGE;
				}
			}

			if (server.argName(i) == "filterTelemetry")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_TELEMETRY;
				}
			}

			if (server.argName(i) == "filterStatus")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_STATUS;
				}
			}

			if (server.argName(i) == "filterWeather")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_WX;
				}
			}

			if (server.argName(i) == "filterObject")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_OBJECT;
				}
			}

			if (server.argName(i) == "filterItem")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_ITEM;
				}
			}

			if (server.argName(i) == "filterQuery")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_QUERY;
				}
			}
			if (server.argName(i) == "filterBuoy")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_BUOY;
				}
			}
			if (server.argName(i) == "filterPosition")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.dispFilter |= FILTER_POSITION;
				}
			}

			if (server.argName(i) == "dispRF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						dispRF = true;
				}
			}

			if (server.argName(i) == "dispINET")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						dispINET = true;
				}
			}
			if (server.argName(i) == "txdispEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						dispTX = true;
				}
			}
			if (server.argName(i) == "rxdispEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						dispRX = true;
				}
			}

			if (server.argName(i) == "dispDelay")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.dispDelay = server.arg(i).toInt();
						if (config.dispDelay < 0)
							config.dispDelay = 0;
					}
				}
			}

			if (server.argName(i) == "oled_timeout")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.oled_timeout = server.arg(i).toInt();
						if (config.oled_timeout < 0)
							config.oled_timeout = 0;
					}
				}
			}
			if (server.argName(i) == "filterDX")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.filterDistant = server.arg(i).toInt();
					}
				}
			}
		}

		config.oled_enable = oledEN;
		config.dispINET = dispINET;
		config.dispRF = dispRF;
		config.rx_display = dispRX;
		config.tx_display = dispTX;
		// config.filterMessage = filterMessage;
		// config.filterStatus = filterStatus;
		// config.filterTelemetry = filterTelemetry;
		// config.filterWeather = filterWeather;
		// config.filterTracker = filterTracker;
		// config.filterMove = filterMove;
		// config.filterPosition = filterPosition;
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else
	{

		struct tm tmstruct;
		char strTime[20];
		tmstruct.tm_year = 0;
		getLocalTime(&tmstruct, 5000);
		sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);

		String html = "<script type=\"text/javascript\">\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		html += "if(e.currentTarget.id===\"formTime\") document.getElementById(\"updateTime\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formNTP\") document.getElementById(\"updateTimeNtp\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formTimeZone\") document.getElementById(\"updateTimeZone\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formReboot\") document.getElementById(\"REBOOT\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formDisp\") document.getElementById(\"submitDISP\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formWebAuth\") document.getElementById(\"submitWebAuth\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/system',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "</script>\n";

		// html += "<h2>System Setting</h2>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>System Setting</b></span></th>\n";
		html += "<tr>";
		// html += "<form accept-charset=\"UTF-8\" action=\"#\" enctype='multipart/form-data' id=\"formTime\" method=\"post\">\n";
		html += "<td style=\"text-align: right;\">LOCAL<br/>DATE/TIME </td>\n";
		html += "<td style=\"text-align: left;\"><br /><form accept-charset=\"UTF-8\" action=\"#\" enctype='multipart/form-data' id=\"formTime\" method=\"post\">\n<input name=\"SetTime\" type=\"text\" value=\"" + String(strTime) + "\" />\n";
		html += "<span class=\"input-group-addon\">\n<span class=\"glyphicon glyphicon-calendar\">\n</span></span>\n";
		// html += "<div class=\"col-sm-3 col-xs-6\"><button class=\"btn btn-primary\" data-args=\"[true]\" data-method=\"getDate\" type=\"button\" data-related-target=\"#SetTime\" />Get Date</button></div>\n";
		html += "<button type='submit' id='updateTime'  name=\"commit\"> Time Update </button>\n";
		html += "<input type=\"hidden\" name=\"updateTime\"/></form>\n</td>\n";
		// html += "<input class=\"btn btn-primary\" id=\"updateTime\" name=\"updateTime\" type=\"submit\" value=\"Time Update\" maxlength=\"80\"/></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td style=\"text-align: right;\">NTP Host </td>\n";
		html += "<td style=\"text-align: left;\"><br /><form accept-charset=\"UTF-8\" action=\"#\" enctype='multipart/form-data' id=\"formNTP\" method=\"post\"><input name=\"SetTimeNtp\" type=\"text\" value=\"" + String(config.ntp_host) + "\" />\n";
		html += "<button type='submit' id='updateTimeNtp'  name=\"commit\"> NTP Update </button>\n";
		html += "<input type=\"hidden\" name=\"updateTimeNtp\"/></form>\n</td>\n";
		// html += "<input class=\"btn btn-primary\" id=\"updateTimeNtp\" name=\"updateTimeNtp\" type=\"submit\" value=\"NTP Update\" maxlength=\"80\"/></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td style=\"text-align: right;\">Time Zone </td>\n";
		html += "<td style=\"text-align: left;\"><br /><form accept-charset=\"UTF-8\" action=\"#\" enctype='multipart/form-data' id=\"formTimeZone\" method=\"post\">\n";
		html += "<select name=\"SetTimeZone\" id=\"SetTimeZone\">\n";
		for (int i = 0; i < 40; i++)
		{
			if (config.timeZone == tzList[i].tz)
				html += "<option value=\"" + String(tzList[i].tz, 1) + "\" selected>" + String(tzList[i].name) + " Sec</option>\n";
			else
				html += "<option value=\"" + String(tzList[i].tz, 1) + "\" >" + String(tzList[i].name) + " Sec</option>\n";
		}
		html += "</select>";
		html += "<button type='submit' id='updateTimeZone'  name=\"commit\"> TZ Update </button>\n";
		html += "<input type=\"hidden\" name=\"updateTimeZone\"/></form>\n</td>\n";
		// html += "<input class=\"btn btn-primary\" id=\"updateTimeZone\" name=\"updateTimeZone\" type=\"submit\" value=\"TZ Update\" maxlength=\"80\"/></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td style=\"text-align: right;\">SYSTEM REBOOT </td>\n";
		html += "<td style=\"text-align: left;\"><br /><form accept-charset=\"UTF-8\" action=\"#\" enctype='multipart/form-data' id=\"formReboot\" method=\"post\"> <button type='submit' id='REBOOT'  name=\"commit\" style=\"background-color:red;color:white\"> REBOOT </button>\n";
		html += " <input type=\"hidden\" name=\"REBOOT\"/></form>\n</td>\n";
		// html += "<td style=\"text-align: left;\"><input type='submit' class=\"btn btn-danger\" id=\"REBOOT\" name=\"REBOOT\" value='REBOOT'></td>\n";
		html += "</tr></table><br /><br />\n";

		/************************ WEB AUTH **************************/
		html += "<form id='formWebAuth' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>Web Authentication</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Web USER:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"32\" maxlength=\"32\" class=\"form-control\" name=\"webauth_user\" type=\"text\" value=\"" + String(config.http_username) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Web PASSWORD:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"63\" maxlength=\"63\" class=\"form-control\" name=\"webauth_pass\" type=\"password\" value=\"" + String(config.http_password) + "\" /></td>\n";
		html += "</tr>\n";
		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitWebAuth'  name=\"commit\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitWebAuth\"/>\n";
		html += "</form><br /><br />";

		html += "<form id='formDisp' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		// html += "<h2>Display Setting</h2>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>Display Setting</b></span></th>\n";
		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>OLED Enable</b></td>\n";
		String oledFlageEn = "";
		if (config.oled_enable == true)
			oledFlageEn = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"oledEnable\" value=\"OK\" " + oledFlageEn + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>TX Display</b></td>\n";
		String txdispFlageEn = "";
		if (config.tx_display == true)
			txdispFlageEn = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"txdispEnable\" value=\"OK\" " + txdispFlageEn + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"> <i>*All TX Packet for display affter filter.</i></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>RX Display</b></td>\n";
		String rxdispFlageEn = "";
		if (config.rx_display == true)
			rxdispFlageEn = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"rxdispEnable\" value=\"OK\" " + rxdispFlageEn + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"> <i>*All RX Packet for display affter filter.</i></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>Head Up</b></td>\n";
		String hupFlageEn = "";
		if (config.h_up == true)
			hupFlageEn = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"hupEnable\" value=\"OK\" " + hupFlageEn + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"> <i>*The compass will rotate in the direction of movement.</i></label></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>Popup Delay</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"dispDelay\" id=\"dispDelay\">\n";
		for (int i = 0; i < 16; i += 1)
		{
			if (config.dispDelay == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(i) + " Sec</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(i) + " Sec</option>\n";
		}
		html += "</select>\n";
		html += "</td></tr>\n";
		html += "<tr>\n";
		html += "<td style=\"text-align: right;\"><b>OLED Sleep</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"oled_timeout\" id=\"oled_timeout\">\n";
		for (int i = 0; i <= 600; i += 30)
		{
			if (config.oled_timeout == i)
				html += "<option value=\"" + String(i) + "\" selected>" + String(i) + " Sec</option>\n";
			else
				html += "<option value=\"" + String(i) + "\" >" + String(i) + " Sec</option>\n";
		}
		html += "</select>\n";
		html += "</td></tr>\n";
		String rfFlageEn = "";
		if (config.dispRF == true)
			rfFlageEn = "checked";
		String inetFlageEn = "";
		if (config.dispINET == true)
			inetFlageEn = "checked";
		html += "<tr><td style=\"text-align: right;\"><b>RX Channel</b></td><td style=\"text-align: left;\"><input type=\"checkbox\" name=\"dispRF\" value=\"OK\" " + rfFlageEn + "/>RF <input type=\"checkbox\" name=\"dispINET\" value=\"OK\" " + inetFlageEn + "/>Internet </td></tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Filter DX:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input type=\"number\" name=\"filterDX\" min=\"0\" max=\"9999\"\n";
		html += "step=\"1\" value=\"" + String(config.filterDistant) + "\" /> Km.  <label style=\"vertical-align: bottom;font-size: 8pt;\"> <i>*Value 0 is all distant allow.</i></label></td>\n";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>Filter:</b></td>\n";

		html += "<td align=\"center\">\n";
		html += "<fieldset id=\"filterDispGrp\">\n";
		html += "<legend>Filter popup display</legend>\n<table style=\"text-align:unset;border-width:0px;background:unset\">";
		html += "<tr style=\"background:unset;\">";

		// html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"dispTNC\" name=\"dispTNC\" type=\"checkbox\" value=\"OK\" " + rfFlageEn + "/>From RF</td>\n";

		// html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"dispINET\" name=\"dispINET\" type=\"checkbox\" value=\"OK\" " + inetFlageEn + "/>From INET</td>\n";

		String filterMessageFlageEn = "";
		if (config.dispFilter & FILTER_MESSAGE)
			filterMessageFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterMessage\" name=\"filterMessage\" type=\"checkbox\" value=\"OK\" " + filterMessageFlageEn + "/>Message</td>\n";

		String filterStatusFlageEn = "";
		if (config.dispFilter & FILTER_STATUS)
			filterStatusFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterStatus\" name=\"filterStatus\" type=\"checkbox\" value=\"OK\" " + filterStatusFlageEn + "/>Status</td>\n";

		String filterTelemetryFlageEn = "";
		if (config.dispFilter & FILTER_TELEMETRY)
			filterTelemetryFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterTelemetry\" name=\"filterTelemetry\" type=\"checkbox\" value=\"OK\" " + filterTelemetryFlageEn + "/>Telemetry</td>\n";

		String filterWeatherFlageEn = "";
		if (config.dispFilter & FILTER_WX)
			filterWeatherFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterWeather\" name=\"filterWeather\" type=\"checkbox\" value=\"OK\" " + filterWeatherFlageEn + "/>Weather</td>\n";

		String filterObjectFlageEn = "";
		if (config.dispFilter & FILTER_OBJECT)
			filterObjectFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterObject\" name=\"filterObject\" type=\"checkbox\" value=\"OK\" " + filterObjectFlageEn + "/>Object</td>\n";

		String filterItemFlageEn = "";
		if (config.dispFilter & FILTER_ITEM)
			filterItemFlageEn = "checked";
		html += "</tr><tr style=\"background:unset;\"><td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterItem\" name=\"filterItem\" type=\"checkbox\" value=\"OK\" " + filterItemFlageEn + "/>Item</td>\n";

		String filterQueryFlageEn = "";
		if (config.dispFilter & FILTER_QUERY)
			filterQueryFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterQuery\" name=\"filterQuery\" type=\"checkbox\" value=\"OK\" " + filterQueryFlageEn + "/>Query</td>\n";

		String filterBuoyFlageEn = "";
		if (config.dispFilter & FILTER_BUOY)
			filterBuoyFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterBuoy\" name=\"filterBuoy\" type=\"checkbox\" value=\"OK\" " + filterBuoyFlageEn + "/>Buoy</td>\n";

		String filterPositionFlageEn = "";
		if (config.dispFilter & FILTER_POSITION)
			filterPositionFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" id=\"filterPosition\" name=\"filterPosition\" type=\"checkbox\" value=\"OK\" " + filterPositionFlageEn + "/>Position</td>\n";

		html += "<td style=\"border:unset;\"></td>";
		html += "</tr></table></fieldset>\n";

		html += "</td></tr></table><br />\n";
		html += "<div><button type='submit' id='submitDISP'  name=\"commitDISP\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitDISP\"/>\n";
		html += "</form><br />";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

void handle_igate()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	bool aprsEn = false;
	bool rf2inetEn = false;
	bool inet2rfEn = false;
	bool posGPS = false;
	bool bcnEN = false;
	bool pos2RF = false;
	bool pos2INET = false;

	if (server.hasArg("commitIGATE"))
	{

		for (int i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "igateEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						aprsEn = true;
				}
			}
			if (server.argName(i) == "myCall")
			{
				if (server.arg(i) != "")
				{
					String name=server.arg(i);
					name.trim();
					strcpy(config.aprs_mycall, name.c_str());
				}
			}
			if (server.argName(i) == "igateObject")
			{
				if (server.arg(i) != "")
				{
					String name=server.arg(i);
					name.trim();
					strcpy(config.igate_object, name.c_str());
				}
				else
				{
					config.igate_object[0] = 0;
				}
			}
			if (server.argName(i) == "mySSID")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.aprs_ssid = server.arg(i).toInt();
					if (config.aprs_ssid > 15)
						config.aprs_ssid = 13;
				}
			}
			if (server.argName(i) == "igatePosInv")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.igate_interval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "igatePosLat")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.igate_lat = server.arg(i).toFloat();
				}
			}

			if (server.argName(i) == "igatePosLon")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.igate_lon = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "igatePosAlt")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.igate_alt = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "igatePosSel")
			{
				if (server.arg(i) != "")
				{
					if (server.arg(i).toInt() == 1)
						posGPS = true;
				}
			}

			if (server.argName(i) == "igateTable")
			{
				if (server.arg(i) != "")
				{
					config.igate_symbol[0] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "igateSymbol")
			{
				if (server.arg(i) != "")
				{
					config.igate_symbol[1] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "aprsHost")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.aprs_host, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "aprsPort")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.aprs_port = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "aprsFilter")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.aprs_filter, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "igatePath")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.igate_path, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "igateComment")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.igate_comment, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "texttouse")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.igate_phg, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "aprsComment")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.igate_comment, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "rf2inetEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						rf2inetEn = true;
				}
			}
			if (server.argName(i) == "inet2rfEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						inet2rfEn = true;
				}
			}
			if (server.argName(i) == "igatePos2RF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2RF = true;
				}
			}
			if (server.argName(i) == "igatePos2INET")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2INET = true;
				}
			}
			if (server.argName(i) == "igateBcnEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						bcnEN = true;
				}
			}
		}

		config.igate_en = aprsEn;
		config.rf2inet = rf2inetEn;
		config.inet2rf = inet2rfEn;
		config.igate_gps = posGPS;
		config.igate_bcn = bcnEN;
		config.igate_loc2rf = pos2RF;
		config.igate_loc2inet = pos2INET;

		saveEEPROM();
		initInterval=true;
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("commitIGATEfilter"))
	{
		config.rf2inetFilter = 0;
		config.inet2rfFilter = 0;
		for (int i = 0; i < server.args(); i++)
		{
			// config rf2inet filter
			if (server.argName(i) == "rf2inetFilterMessage")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_MESSAGE;
				}
			}

			if (server.argName(i) == "rf2inetFilterTelemetry")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_TELEMETRY;
				}
			}

			if (server.argName(i) == "rf2inetFilterStatus")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_STATUS;
				}
			}

			if (server.argName(i) == "rf2inetFilterWeather")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_WX;
				}
			}

			if (server.argName(i) == "rf2inetFilterObject")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_OBJECT;
				}
			}

			if (server.argName(i) == "rf2inetFilterItem")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_ITEM;
				}
			}

			if (server.argName(i) == "rf2inetFilterQuery")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_QUERY;
				}
			}
			if (server.argName(i) == "rf2inetFilterBuoy")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_BUOY;
				}
			}
			if (server.argName(i) == "rf2inetFilterPosition")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.rf2inetFilter |= FILTER_POSITION;
				}
			}
			// config inet2rf filter

			if (server.argName(i) == "inet2rfFilterMessage")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_MESSAGE;
				}
			}

			if (server.argName(i) == "inet2rfFilterTelemetry")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_TELEMETRY;
				}
			}

			if (server.argName(i) == "inet2rfFilterStatus")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_STATUS;
				}
			}

			if (server.argName(i) == "inet2rfFilterWeather")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_WX;
				}
			}

			if (server.argName(i) == "inet2rfFilterObject")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_OBJECT;
				}
			}

			if (server.argName(i) == "inet2rfFilterItem")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_ITEM;
				}
			}

			if (server.argName(i) == "inet2rfFilterQuery")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_QUERY;
				}
			}
			if (server.argName(i) == "inet2rfFilterBuoy")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_BUOY;
				}
			}
			if (server.argName(i) == "inet2rfFilterPosition")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.inet2rfFilter |= FILTER_POSITION;
				}
			}
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}else{
		String html = "<script type=\"text/javascript\">\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		// html += "document.getElementById(\"submitIGATE\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formIgate\") document.getElementById(\"submitIGATE\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formIgateFilter\") document.getElementById(\"submitIGATEfilter\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/igate',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "</script>\n<script type=\"text/javascript\">\n";
		html += "function openWindowSymbol() {\n";
		html += "var i, l, options = [{\n";
		html += "value: 'first',\n";
		html += "text: 'First'\n";
		html += "}, {\n";
		html += "value: 'second',\n";
		html += "text: 'Second'\n";
		html += "}],\n";
		html += "newWindow = window.open(\"\", null, \"height=400,width=400,status=no,toolbar=no,menubar=no,location=no\");\n";

		int i;

		html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
		html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '/'</th></tr><tr>\");\n";
		for (i = 33; i < 129; i++)
		{
			html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(" + String(i) + ",1);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i) + "-1.png\\\"></td>\");\n";
			if (((i % 16) == 0) && (i < 126))
				html += "newWindow.document.write(\"</tr><tr>\");\n";
		}
		html += "newWindow.document.write(\"</tr></table><br />\");\n";
		html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
		html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '\\\'</th></tr><tr>\");\n";
		for (i = 33; i < 129; i++)
		{
			html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(" + String(i) + ",2);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i, DEC) + "-2.png\\\"></td>\");\n";
			if (((i % 16) == 0) && (i < 126))
				html += "newWindow.document.write(\"</tr><tr>\");\n";
		}
		html += "newWindow.document.write(\"</tr></table>\");\n";

		// html += "newWindow.document.write(\"</select>\");\");\n";
		html += "}\n";

		html += "function setValue(symbol,table) {\n";
		html += "document.getElementById('igateSymbol').value = String.fromCharCode(symbol);\n";
		html += "if(table==1){\n document.getElementById('igateTable').value='/';\n";
		html += "}else if(table==2){\n document.getElementById('igateTable').value='\\\\';\n}\n";
		html += "document.getElementById('igateImgSymbol').src = \"http://www.dprns.com/symbols/icons/\"+symbol.toString()+'-'+table.toString();\n";
		html += "\n}\n";
		html += "function calculatePHGR(){document.forms.formIgate.texttouse.value=\"PHG\"+calcPower(document.forms.formIgate.power.value)+calcHeight(document.forms.formIgate.haat.value)+calcGain(document.forms.formIgate.gain.value)+calcDirection(document.forms.formIgate.direction.selectedIndex)}function Log2(e){return Math.log(e)/Math.log(2)}function calcPerHour(e){return e<10?e:String.fromCharCode(65+(e-10))}function calcHeight(e){return String.fromCharCode(48+Math.round(Log2(e/10),0))}function calcPower(e){if(e<1)return 0;if(e>=1&&e<4)return 1;if(e>=4&&e<9)return 2;if(e>=9&&e<16)return 3;if(e>=16&&e<25)return 4;if(e>=25&&e<36)return 5;if(e>=36&&e<49)return 6;if(e>=49&&e<64)return 7;if(e>=64&&e<81)return 8;if(e>=81)return 9}function calcDirection(e){if(e==\"0\")return\"0\";if(e==\"1\")return\"1\";if(e==\"2\")return\"2\";if(e==\"3\")return\"3\";if(e==\"4\")return\"4\";if(e==\"5\")return\"5\";if(e==\"6\")return\"6\";if(e==\"7\")return\"7\";if(e==\"8\")return\"8\"}function calcGain(e){return e>9?\"9\":e<0?\"0\":Math.round(e,0)}\n";
		html += "function onRF2INETCheck() {\n";
		html += "if (document.querySelector('#rf2inetEnable').checked) {\n";
		// Checkbox has been checked
		html += "document.getElementById(\"rf2inetFilterGrp\").disabled=false;\n";
		html += "} else {\n";
		// Checkbox has been unchecked
		html += "document.getElementById(\"rf2inetFilterGrp\").disabled=true;\n";
		html += "}\n}\n";
		html += "function onINET2RFCheck() {\n";
		html += "if (document.querySelector('#inet2rfEnable').checked) {\n";
		// Checkbox has been checked
		html += "document.getElementById(\"inet2rfFilterGrp\").disabled=false;\n";
		html += "} else {\n";
		// Checkbox has been unchecked
		html += "document.getElementById(\"inet2rfFilterGrp\").disabled=true;\n";
		html += "}\n}\n";
		html += "</script>\n";

		/************************ IGATE Mode **************************/
		html += "<form id='formIgate' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		// html += "<h2>[IGATE] Internet Gateway Mode</h2>\n";
		html += "<table>\n";
		// html += "<tr>\n";
		// html += "<th width=\"200\"><span><b>Setting</b></span></th>\n";
		// html += "<th><span><b>Value</b></span></th>\n";
		// html += "</tr>\n";
		html += "<th colspan=\"2\"><span><b>[IGATE] Internet Gateway Mode</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Enable:</b></td>\n";
		String igateEnFlag = "";
		if (config.igate_en)
			igateEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"igateEnable\" value=\"OK\" " + igateEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station Callsign:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"7\" size=\"6\" id=\"myCall\" name=\"myCall\" type=\"text\" value=\"" + String(config.aprs_mycall) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station SSID:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"mySSID\" id=\"mySSID\">\n";
		for (uint8_t ssid = 0; ssid <= 15; ssid++)
		{
			if (config.aprs_ssid == ssid)
			{
				html += "<option value=\"" + String(ssid) + "\" selected>" + String(ssid) + "</option>\n";
			}
			else
			{
				html += "<option value=\"" + String(ssid) + "\">" + String(ssid) + "</option>\n";
			}
		}
		html += "</select></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station Symbol:</b></td>\n";
		String table = "1";
		if (config.igate_symbol[0] == 47)
			table = "1";
		if (config.igate_symbol[0] == 92)
			table = "2";
		html += "<td style=\"text-align: left;\">Table:<input maxlength=\"1\" size=\"1\" id=\"igateTable\" name=\"igateTable\" type=\"text\" value=\"" + String(config.igate_symbol[0]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> Symbol:<input maxlength=\"1\" size=\"1\" id=\"igateSymbol\" name=\"igateSymbol\" type=\"text\" value=\"" + String(config.igate_symbol[1]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> <img border=\"1\" style=\"vertical-align: middle;\" id=\"igateImgSymbol\" onclick=\"openWindowSymbol();\" src=\"http://www.dprns.com/symbols/icons/" + String((int)config.igate_symbol[1]) + "-" + table + ".png\"> <i>*Click icon for select symbol</i></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Item/Obj Name:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"9\" size=\"9\" id=\"igateObject\" name=\"igateObject\" type=\"text\" value=\"" + String(config.igate_object) + "\" /><i> *If not used, leave it blank.In use 3-9 charactor</i></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>PATH:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"72\" size=\"72\" id=\"igatePath\" name=\"igatePath\" type=\"text\" value=\"" + String(config.igate_path) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Server Host:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"20\" size=\"20\" id=\"aprsHost\" name=\"aprsHost\" type=\"text\" value=\"" + String(config.aprs_host) + "\" /> *Support APRS-IS of T2THAI at <a href=\"http://aprs.dprns.com:14501\" target=\"_t2thai\">aprs.dprns.com:14580</a></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Server Port:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input min=\"1\" max=\"65535\" step=\"1\" id=\"aprsPort\" name=\"aprsPort\" type=\"number\" value=\"" + String(config.aprs_port) + "\" /> *Support AMPR Host at <a href=\"http://aprs.hs5tqa.ampr.org:14501\" target=\"_t2thai\">aprs.hs5tqa.ampr.org:14580</a></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Server Filter:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"30\" size=\"30\" id=\"aprsFilter\" name=\"aprsFilter\" type=\"text\" value=\"" + String(config.aprs_filter) + "\" /> *Filter: <a target=\"_blank\" href=\"http://www.aprs-is.net/javAPRSFilter.aspx\">http://www.aprs-is.net/javAPRSFilter.aspx</a></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Text Comment:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"50\" size=\"50\" id=\"igateComment\" name=\"igateComment\" type=\"text\" value=\"" + String(config.igate_comment) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>RF2INET:</b></td>\n";
		String rf2inetEnFlag = "";
		if (config.rf2inet)
			rf2inetEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" id=\"rf2inetEnable\" name=\"rf2inetEnable\" onclick=\"onRF2INETCheck()\" value=\"OK\" " + rf2inetEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Switch RF to Internet gateway</i></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>INET2RF:</b></td>\n";
		String inet2rfEnFlag = "";
		if (config.inet2rf)
			inet2rfEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" id=\"inet2rfEnable\" name=\"inet2rfEnable\" onclick=\"onINET2RFCheck()\" value=\"OK\" " + inet2rfEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Switch Internet to RF gateway</i></label></td>\n";
		html += "</tr>\n<tr>";

		html += "<td align=\"right\"><b>POSITION:</b></td>\n";
		html += "<td align=\"center\">\n";
		html += "<table>";
		String igateBcnEnFlag = "";
		if (config.igate_bcn)
			igateBcnEnFlag = "checked";

		html += "<tr><td style=\"text-align: right;\">Beacon:</td><td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"igateBcnEnable\" value=\"OK\" " + igateBcnEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\">  Interval:<input min=\"0\" max=\"3600\" step=\"1\" id=\"igatePosInv\" name=\"igatePosInv\" type=\"number\" value=\"" + String(config.igate_interval) + "\" />Sec.</label></td></tr>";
		String igatePosFixFlag = "";
		String igatePosGPSFlag = "";
		String igatePos2RFFlag = "";
		String igatePos2INETFlag = "";
		if (config.igate_gps)
			igatePosGPSFlag = "checked=\"checked\"";
		else
			igatePosFixFlag = "checked=\"checked\"";

		if (config.igate_loc2rf)
			igatePos2RFFlag = "checked";
		if (config.igate_loc2inet)
			igatePos2INETFlag = "checked";
		html += "<tr><td style=\"text-align: right;\">Location:</td><td style=\"text-align: left;\"><input type=\"radio\" name=\"igatePosSel\" value=\"0\" " + igatePosFixFlag + "/>Fix <input type=\"radio\" name=\"igatePosSel\" value=\"1\" " + igatePosGPSFlag + "/>GPS </td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">TX Channel:</td><td style=\"text-align: left;\"><input type=\"checkbox\" name=\"igatePos2RF\" value=\"OK\" " + igatePos2RFFlag + "/>RF <input type=\"checkbox\" name=\"igatePos2INET\" value=\"OK\" " + igatePos2INETFlag + "/>Internet </td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Latitude:</td><td style=\"text-align: left;\"><input min=\"-90\" max=\"90\" step=\"0.0001\" id=\"igatePosLat\" name=\"igatePosLat\" type=\"number\" value=\"" + String(config.igate_lat, 5) + "\" />degrees (positive for North, negative for South)</td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Longitude:</td><td style=\"text-align: left;\"><input min=\"-180\" max=\"180\" step=\"0.0001\" id=\"igatePosLon\" name=\"igatePosLon\" type=\"number\" value=\"" + String(config.igate_lon, 5) + "\" />degrees (positive for East, negative for West)</td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Altitude:</td><td style=\"text-align: left;\"><input min=\"0\" max=\"10000\" step=\"0.1\" id=\"igatePosAlt\" name=\"igatePosAlt\" type=\"number\" value=\"" + String(config.igate_alt, 2) + "\" /> meter. *Value 0 is not send height</td></tr>\n";
		html += "</table></td>";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>PHG:</b></td>\n";
		html += "<td align=\"center\">\n";
		html += "<table>";
		html += "<tr>\n";
		html += "<td align=\"right\">Radio TX Power</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"power\" id=\"power\">\n";
		html += "<option value=\"1\" selected>1</option>\n";
		html += "<option value=\"5\">5</option>\n";
		html += "<option value=\"10\">10</option>\n";
		html += "<option value=\"15\">15</option>\n";
		html += "<option value=\"25\">25</option>\n";
		html += "<option value=\"35\">35</option>\n";
		html += "<option value=\"50\">50</option>\n";
		html += "<option value=\"65\">65</option>\n";
		html += "<option value=\"80\">80</option>\n";
		html += "</select> Watts</td>\n";
		html += "</tr>\n";
		html += "<tr><td style=\"text-align: right;\">Antenna Gain</td><td style=\"text-align: left;\"><input size=\"3\" min=\"0\" max=\"100\" step=\"0.1\" id=\"gain\" name=\"gain\" type=\"number\" value=\"6\" /> dBi</td></tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\">Height</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"haat\" id=\"haat\">\n";
		int k = 10;
		for (uint8_t w = 0; w < 10; w++)
		{
			if (w == 0)
			{
				html += "<option value=\"" + String(k) + "\" selected>" + String(k) + "</option>\n";
			}
			else
			{
				html += "<option value=\"" + String(k) + "\">" + String(k) + "</option>\n";
			}
			k += k;
		}
		html += "</select> Feet</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\">Antenna/Direction</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"direction\" id=\"direction\">\n";
		html += "<option>Omni</option><option>NE</option><option>E</option><option>SE</option><option>S</option><option>SW</option><option>W</option><option>NW</option><option>N</option>\n";
		html += "</select></td>\n";
		html += "</tr>\n";

		html += "<tr><td align=\"right\"><b>PHG Text</b></td><td align=\"left\"><input name=\"texttouse\" type=\"text\" size=\"6\" style=\"background-color: rgb(97, 239, 170);\" value=\"" + String(config.igate_phg) + "\"/> <input type=\"button\" value=\"Calculate PHG\" onclick=\"javascript:calculatePHGR()\" /></td></tr>\n";
		html += "</table></td>";
		html += "</tr>\n";

		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitIGATE'  name=\"commitIGATE\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitIGATE\"/>\n";
		html += "</form><br /><br />";

		html += "<form id='formIgateFilter' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>[IGATE] Filter</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>RF2INET Filter:</b></td>\n";

		html += "<td align=\"center\">\n";
		if (config.rf2inet)
			html += "<fieldset id=\"rf2inetFilterGrp\">\n";
		else
			html += "<fieldset id=\"rf2inetFilterGrp\" disabled>\n";
		html += "<legend>Filter RF to Internet</legend>\n<table style=\"text-align:unset;border-width:0px;background:unset\">";
		html += "<tr style=\"background:unset;\">";

		String filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_MESSAGE)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterMessage\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Message</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_STATUS)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterStatus\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Status</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_TELEMETRY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterTelemetry\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Telemetry</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_WX)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterWeather\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Weather</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_OBJECT)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterObject\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Object</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_ITEM)
			filterFlageEn = "checked";
		html += "</tr><tr style=\"background:unset;\"><td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterItem\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Item</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_QUERY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterQuery\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Query</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_BUOY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterBuoy\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Buoy</td>\n";

		filterFlageEn = "";
		if (config.rf2inetFilter & FILTER_POSITION)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"rf2inetFilterPosition\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Position</td>\n";

		html += "<td style=\"border:unset;\"></td>";
		html += "</tr></table></fieldset>\n";
		html += "</td></tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>INET2RF Filter:</b></td>\n";

		html += "<td align=\"center\">\n";
		if (config.inet2rf)
			html += "<fieldset id=\"inet2rfFilterGrp\">\n";
		else
			html += "<fieldset id=\"inet2rfFilterGrp\" disabled>\n";
		html += "<legend>Filter Internet to RF</legend>\n<table style=\"text-align:unset;border-width:0px;background:unset\">";
		html += "<tr style=\"background:unset;\">";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_MESSAGE)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterMessage\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Message</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_STATUS)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterStatus\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Status</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_TELEMETRY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterTelemetry\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Telemetry</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_WX)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterWeather\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Weather</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_OBJECT)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterObject\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Object</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_ITEM)
			filterFlageEn = "checked";
		html += "</tr><tr style=\"background:unset;\"><td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterItem\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Item</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_QUERY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterQuery\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Query</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_BUOY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterBuoy\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Buoy</td>\n";

		filterFlageEn = "";
		if (config.inet2rfFilter & FILTER_POSITION)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"inet2rfFilterPosition\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Position</td>\n";

		html += "<td style=\"border:unset;\"></td>";
		html += "</tr></table></fieldset>\n";
		html += "</td></tr>\n";

		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitIGATEfilter'  name=\"commitIGATEfilter\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitIGATEfilter\"/>\n";
		html += "</form><br />";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

void handle_digi()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	bool digiEn = false;
	bool posGPS = false;
	bool bcnEN = false;
	bool pos2RF = false;
	bool pos2INET = false;

	if (server.hasArg("commitDIGI"))
	{
		config.digiFilter = 0;
		for (int i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "digiEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						digiEn = true;
				}
			}
			if (server.argName(i) == "myCall")
			{
				if (server.arg(i) != "")
				{
					String name=server.arg(i);
					name.trim();
					strcpy(config.digi_mycall, name.c_str());
				}
			}
			if (server.argName(i) == "mySSID")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_ssid = server.arg(i).toInt();
					if (config.digi_ssid > 15)
						config.digi_ssid = 3;
				}
			}
			if (server.argName(i) == "digiDelay")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_delay = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "digiPosInv")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_interval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "digiPosLat")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_lat = server.arg(i).toFloat();
				}
			}

			if (server.argName(i) == "digiPosLon")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_lon = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "digiPosAlt")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.digi_alt = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "digiPosSel")
			{
				if (server.arg(i) != "")
				{
					if (server.arg(i).toInt() == 1)
						posGPS = true;
				}
			}

			if (server.argName(i) == "digiTable")
			{
				if (server.arg(i) != "")
				{
					config.digi_symbol[0] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "digiSymbol")
			{
				if (server.arg(i) != "")
				{
					config.digi_symbol[1] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "digiPath")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.digi_path, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "digiComment")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.digi_comment, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "texttouse")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.digi_phg, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "digiComment")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.digi_comment, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "digiPos2RF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2RF = true;
				}
			}
			if (server.argName(i) == "digiPos2INET")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2INET = true;
				}
			}
			if (server.argName(i) == "digiBcnEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						bcnEN = true;
				}
			}
			// Filter
			if (server.argName(i) == "FilterMessage")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_MESSAGE;
				}
			}

			if (server.argName(i) == "FilterTelemetry")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_TELEMETRY;
				}
			}

			if (server.argName(i) == "FilterStatus")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_STATUS;
				}
			}

			if (server.argName(i) == "FilterWeather")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_WX;
				}
			}

			if (server.argName(i) == "FilterObject")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_OBJECT;
				}
			}

			if (server.argName(i) == "FilterItem")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_ITEM;
				}
			}

			if (server.argName(i) == "FilterQuery")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_QUERY;
				}
			}
			if (server.argName(i) == "FilterBuoy")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_BUOY;
				}
			}
			if (server.argName(i) == "FilterPosition")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						config.digiFilter |= FILTER_POSITION;
				}
			}
		}
		config.digi_en = digiEn;
		config.digi_gps = posGPS;
		config.digi_bcn = bcnEN;
		config.digi_loc2rf = pos2RF;
		config.digi_loc2inet = pos2INET;

		saveEEPROM();
		initInterval=true;
		String html = "OK";
		server.send(200, "text/html", html);
	}else{
		String html = "<script type=\"text/javascript\">\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		html += "document.getElementById(\"submitDIGI\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/digi',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "</script>\n<script type=\"text/javascript\">\n";
		html += "function openWindowSymbol() {\n";
		html += "var i, l, options = [{\n";
		html += "value: 'first',\n";
		html += "text: 'First'\n";
		html += "}, {\n";
		html += "value: 'second',\n";
		html += "text: 'Second'\n";
		html += "}],\n";
		html += "newWindow = window.open(\"\", null, \"height=400,width=400,status=no,toolbar=no,menubar=no,titlebar=no,location=no\");\n";

		int i;

		html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
		html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '/'</th></tr><tr>\");\n";
		for (i = 33; i < 129; i++)
		{
			html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(" + String(i) + ",1);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i) + "-1.png\\\"></td>\");\n";
			if (((i % 16) == 0) && (i < 126))
				html += "newWindow.document.write(\"</tr><tr>\");\n";
		}
		html += "newWindow.document.write(\"</tr></table><br />\");\n";
		html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
		html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '\\\'</th></tr><tr>\");\n";
		for (i = 33; i < 129; i++)
		{
			html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(" + String(i) + ",2);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i, DEC) + "-2.png\\\"></td>\");\n";
			if (((i % 16) == 0) && (i < 126))
				html += "newWindow.document.write(\"</tr><tr>\");\n";
		}
		html += "newWindow.document.write(\"</tr></table>\");\n";
		html += "}\n";

		html += "function setValue(symbol,table) {\n";
		html += "document.getElementById('digiSymbol').value = String.fromCharCode(symbol);\n";
		html += "if(table==1){\n document.getElementById('digiTable').value='/';\n";
		html += "}else if(table==2){\n document.getElementById('digiTable').value='\\\\';\n}\n";
		html += "document.getElementById('digiImgSymbol').src = \"http://www.dprns.com/symbols/icons/\"+symbol.toString()+'-'+table.toString();\n";
		html += "\n}\n";
		html += "function calculatePHGR(){document.forms.formDIGI.texttouse.value=\"PHG\"+calcPower(document.forms.formDIGI.power.value)+calcHeight(document.forms.formDIGI.haat.value)+calcGain(document.forms.formDIGI.gain.value)+calcDirection(document.forms.formDIGI.direction.selectedIndex)}function Log2(e){return Math.log(e)/Math.log(2)}function calcPerHour(e){return e<10?e:String.fromCharCode(65+(e-10))}function calcHeight(e){return String.fromCharCode(48+Math.round(Log2(e/10),0))}function calcPower(e){if(e<1)return 0;if(e>=1&&e<4)return 1;if(e>=4&&e<9)return 2;if(e>=9&&e<16)return 3;if(e>=16&&e<25)return 4;if(e>=25&&e<36)return 5;if(e>=36&&e<49)return 6;if(e>=49&&e<64)return 7;if(e>=64&&e<81)return 8;if(e>=81)return 9}function calcDirection(e){if(e==\"0\")return\"0\";if(e==\"1\")return\"1\";if(e==\"2\")return\"2\";if(e==\"3\")return\"3\";if(e==\"4\")return\"4\";if(e==\"5\")return\"5\";if(e==\"6\")return\"6\";if(e==\"7\")return\"7\";if(e==\"8\")return\"8\"}function calcGain(e){return e>9?\"9\":e<0?\"0\":Math.round(e,0)}\n";
		html += "</script>\n";

		/************************ DIGI Mode **************************/
		html += "<form id='formDIGI' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		// html += "<h2>[DIGI] Digital Repeater Mode</h2>\n";
		html += "<table>\n";
		// html += "<tr>\n";
		// html += "<th width=\"200\"><span><b>Setting</b></span></th>\n";
		// html += "<th><span><b>Value</b></span></th>\n";
		// html += "</tr>\n";
		html += "<th colspan=\"2\"><span><b>[DIGI] Dital Repeater Mode</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Enable:</b></td>\n";
		String digiEnFlag = "";
		if (config.digi_en)
			digiEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"digiEnable\" value=\"OK\" " + digiEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station Callsign:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"7\" size=\"6\" id=\"myCall\" name=\"myCall\" type=\"text\" value=\"" + String(config.digi_mycall) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station SSID:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"mySSID\" id=\"mySSID\">\n";
		for (uint8_t ssid = 0; ssid <= 15; ssid++)
		{
			if (config.digi_ssid == ssid)
			{
				html += "<option value=\"" + String(ssid) + "\" selected>" + String(ssid) + "</option>\n";
			}
			else
			{
				html += "<option value=\"" + String(ssid) + "\">" + String(ssid) + "</option>\n";
			}
		}
		html += "</select></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Station Symbol:</b></td>\n";
		String table = "1";
		if (config.digi_symbol[0] == 47)
			table = "1";
		if (config.digi_symbol[0] == 92)
			table = "2";
		html += "<td style=\"text-align: left;\">Table:<input maxlength=\"1\" size=\"1\" id=\"digiTable\" name=\"digiTable\" type=\"text\" value=\"" + String(config.digi_symbol[0]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> Symbol:<input maxlength=\"1\" size=\"1\" id=\"digiSymbol\" name=\"digiSymbol\" type=\"text\" value=\"" + String(config.digi_symbol[1]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> <img border=\"1\" style=\"vertical-align: middle;\" id=\"digiImgSymbol\" onclick=\"openWindowSymbol();\" src=\"http://www.dprns.com/symbols/icons/" + String((int)config.digi_symbol[1]) + "-" + table + ".png\"> <i>*Click icon for select symbol</i></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>PATH:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"72\" size=\"72\" id=\"digiPath\" name=\"digiPath\" type=\"text\" value=\"" + String(config.digi_path) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Text Comment:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"50\" size=\"50\" id=\"digiComment\" name=\"digiComment\" type=\"text\" value=\"" + String(config.digi_comment) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<tr><td style=\"text-align: right;\"><b>Repeat Delay:</b></td><td style=\"text-align: left;\"><input min=\"0\" max=\"10000\" step=\"100\" id=\"digiDelay\" name=\"digiDelay\" type=\"number\" value=\"" + String(config.digi_delay) + "\" /> mSec. <i>*0 is auto,Other random of delay time</i></td></tr>";

		html += "<tr><td align=\"right\"><b>POSITION:</b></td>\n";
		html += "<td align=\"center\">\n";
		html += "<table>";
		String digiBcnEnFlag = "";
		if (config.digi_bcn)
			digiBcnEnFlag = "checked";

		html += "<tr><td style=\"text-align: right;\">Beacon:</td><td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"digiBcnEnable\" value=\"OK\" " + digiBcnEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\">  Interval:<input min=\"0\" max=\"3600\" step=\"1\" id=\"digiPosInv\" name=\"digiPosInv\" type=\"number\" value=\"" + String(config.digi_interval) + "\" />Sec.</label></td></tr>";
		String digiPosFixFlag = "";
		String digiPosGPSFlag = "";
		String digiPos2RFFlag = "";
		String digiPos2INETFlag = "";
		if (config.digi_gps)
			digiPosGPSFlag = "checked=\"checked\"";
		else
			digiPosFixFlag = "checked=\"checked\"";

		if (config.digi_loc2rf)
			digiPos2RFFlag = "checked";
		if (config.digi_loc2inet)
			digiPos2INETFlag = "checked";
		html += "<tr><td style=\"text-align: right;\">Location:</td><td style=\"text-align: left;\"><input type=\"radio\" name=\"digiPosSel\" value=\"0\" " + digiPosFixFlag + "/>Fix <input type=\"radio\" name=\"digiPosSel\" value=\"1\" " + digiPosGPSFlag + "/>GPS </td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">TX Channel:</td><td style=\"text-align: left;\"><input type=\"checkbox\" name=\"digiPos2RF\" value=\"OK\" " + digiPos2RFFlag + "/>RF <input type=\"checkbox\" name=\"digiPos2INET\" value=\"OK\" " + digiPos2INETFlag + "/>Internet </td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Latitude:</td><td style=\"text-align: left;\"><input min=\"-90\" max=\"90\" step=\"0.0001\" id=\"digiPosLat\" name=\"digiPosLat\" type=\"number\" value=\"" + String(config.digi_lat, 5) + "\" />degrees (positive for North, negative for South)</td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Longitude:</td><td style=\"text-align: left;\"><input min=\"-180\" max=\"180\" step=\"0.0001\" id=\"digiPosLon\" name=\"digiPosLon\" type=\"number\" value=\"" + String(config.digi_lon, 5) + "\" />degrees (positive for East, negative for West)</td></tr>\n";
		html += "<tr><td style=\"text-align: right;\">Altitude:</td><td style=\"text-align: left;\"><input min=\"0\" max=\"10000\" step=\"0.1\" id=\"digiPosAlt\" name=\"digiPosAlt\" type=\"number\" value=\"" + String(config.digi_alt, 2) + "\" /> meter. *Value 0 is not send height</td></tr>\n";
		html += "</table></td>";
		html += "</tr>\n";

		html += "<tr>\n";
		html += "<td align=\"right\"><b>PHG:</b></td>\n";
		html += "<td align=\"center\">\n";
		html += "<table>";
		html += "<tr>\n";
		html += "<td align=\"right\">Radio TX Power</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"power\" id=\"power\">\n";
		html += "<option value=\"1\" selected>1</option>\n";
		html += "<option value=\"5\">5</option>\n";
		html += "<option value=\"10\">10</option>\n";
		html += "<option value=\"15\">15</option>\n";
		html += "<option value=\"25\">25</option>\n";
		html += "<option value=\"35\">35</option>\n";
		html += "<option value=\"50\">50</option>\n";
		html += "<option value=\"65\">65</option>\n";
		html += "<option value=\"80\">80</option>\n";
		html += "</select> Watts</td>\n";
		html += "</tr>\n";
		html += "<tr><td style=\"text-align: right;\">Antenna Gain</td><td style=\"text-align: left;\"><input size=\"3\" min=\"0\" max=\"100\" step=\"0.1\" id=\"gain\" name=\"gain\" type=\"number\" value=\"6\" /> dBi</td></tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\">Height</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"haat\" id=\"haat\">\n";
		int k = 10;
		for (uint8_t w = 0; w < 10; w++)
		{
			if (w == 0)
			{
				html += "<option value=\"" + String(k) + "\" selected>" + String(k) + "</option>\n";
			}
			else
			{
				html += "<option value=\"" + String(k) + "\">" + String(k) + "</option>\n";
			}
			k += k;
		}
		html += "</select> Feet</td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\">Antenna/Direction</td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"direction\" id=\"direction\">\n";
		html += "<option>Omni</option><option>NE</option><option>E</option><option>SE</option><option>S</option><option>SW</option><option>W</option><option>NW</option><option>N</option>\n";
		html += "</select></td>\n";
		html += "</tr>\n";
		html += "<tr><td align=\"right\"><b>PHG Text</b></td><td align=\"left\"><input name=\"texttouse\" type=\"text\" size=\"6\" style=\"background-color: rgb(97, 239, 170);\" value=\"" + String(config.digi_phg) + "\"/> <input type=\"button\" value=\"Calculate PHG\" onclick=\"javascript:calculatePHGR()\" /></td></tr>\n";

		html += "</table></tr>";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Filter:</b></td>\n";

		html += "<td align=\"center\">\n";
		html += "<fieldset id=\"FilterGrp\">\n";
		html += "<legend>Filter repeater</legend>\n<table style=\"text-align:unset;border-width:0px;background:unset\">";
		html += "<tr style=\"background:unset;\">";

		String filterFlageEn = "";
		if (config.digiFilter & FILTER_MESSAGE)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterMessage\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Message</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_STATUS)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterStatus\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Status</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_TELEMETRY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterTelemetry\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Telemetry</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_WX)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterWeather\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Weather</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_OBJECT)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterObject\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Object</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_ITEM)
			filterFlageEn = "checked";
		html += "</tr><tr style=\"background:unset;\"><td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterItem\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Item</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_QUERY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterQuery\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Query</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_BUOY)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterBuoy\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Buoy</td>\n";

		filterFlageEn = "";
		if (config.digiFilter & FILTER_POSITION)
			filterFlageEn = "checked";
		html += "<td style=\"border:unset;\"><input class=\"field_checkbox\" name=\"FilterPosition\" type=\"checkbox\" value=\"OK\" " + filterFlageEn + "/>Position</td>\n";

		html += "<td style=\"border:unset;\"></td>";
		html += "</tr></table></fieldset>\n";
		html += "</td></tr>\n";
		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitDIGI'  name=\"commitDIGI\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitDIGI\"/>\n";
		html += "</form><br />";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

void handle_tracker()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	bool trakerEn = false;
	bool smartEn = false;
	bool compEn = false;

	bool posGPS = false;
	bool bcnEN = false;
	bool pos2RF = false;
	bool pos2INET = false;
	bool optCST = false;
	bool optAlt = false;
	bool optBat = false;
	bool optSat = false;

	if (server.hasArg("commitTRACKER"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "trackerEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						trakerEn = true;
				}
			}
			if (server.argName(i) == "smartBcnEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						smartEn = true;
				}
			}
			if (server.argName(i) == "compressEnable")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						compEn = true;
				}
			}
			if (server.argName(i) == "trackerOptCST")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						optCST = true;
				}
			}
			if (server.argName(i) == "trackerOptAlt")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						optAlt = true;
				}
			}
			if (server.argName(i) == "trackerOptBat")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						optBat = true;
				}
			}
			if (server.argName(i) == "trackerOptSat")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						optSat = true;
				}
			}
			if (server.argName(i) == "myCall")
			{
				if (server.arg(i) != "")
				{
					String name=server.arg(i);
					name.trim();
					strcpy(config.trk_mycall, name.c_str());
				}
			}
			if (server.argName(i) == "trackerObject")
			{
				if (server.arg(i) != "")
				{
					String name=server.arg(i);
					name.trim();
					strcpy(config.trk_item, name.c_str());
				}
				else
				{
					config.trk_item[0] = 0;
				}
			}
			if (server.argName(i) == "mySSID")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_ssid = server.arg(i).toInt();
					if (config.trk_ssid > 15)
						config.trk_ssid = 13;
				}
			}
			if (server.argName(i) == "trackerPosInv")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_interval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "trackerPosLat")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_lat = server.arg(i).toFloat();
				}
			}

			if (server.argName(i) == "trackerPosLon")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_lon = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "trackerPosAlt")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_alt = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "trackerPosSel")
			{
				if (server.arg(i) != "")
				{
					if (server.arg(i).toInt() == 1)
						posGPS = true;
				}
			}
			if (server.argName(i) == "hspeed")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_hspeed = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "lspeed")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_lspeed = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "slowInterval")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_slowinterval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "maxInterval")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_maxinterval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "minInterval")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_mininterval = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "minAngle")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.trk_minangle = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "trackerTable")
			{
				if (server.arg(i) != "")
				{
					config.trk_symbol[0] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "trackerSymbol")
			{
				if (server.arg(i) != "")
				{
					config.trk_symbol[1] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "moveTable")
			{
				if (server.arg(i) != "")
				{
					config.trk_symmove[0] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "moveSymbol")
			{
				if (server.arg(i) != "")
				{
					config.trk_symmove[1] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "stopTable")
			{
				if (server.arg(i) != "")
				{
					config.trk_symstop[0] = server.arg(i).charAt(0);
				}
			}
			if (server.argName(i) == "stopSymbol")
			{
				if (server.arg(i) != "")
				{
					config.trk_symstop[1] = server.arg(i).charAt(0);
				}
			}

			if (server.argName(i) == "trackerPath")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.trk_path, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "trackerComment")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.trk_comment, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "trackerPos2RF")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2RF = true;
				}
			}
			if (server.argName(i) == "trackerPos2INET")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
						pos2INET = true;
				}
			}
		}
		config.trk_en = trakerEn;
		config.trk_smartbeacon = smartEn;
		config.trk_compress = compEn;

		config.trk_gps = posGPS;
		config.trk_loc2rf = pos2RF;
		config.trk_loc2inet = pos2INET;

		config.trk_cst = optCST;
		config.trk_altitude = optAlt;
		config.trk_bat = optBat;
		config.trk_sat = optSat;

		saveEEPROM();
		initInterval=true;
		String html = "OK";
		server.send(200, "text/html", html);
	}

	String html = "<script type=\"text/javascript\">\n";
	html += "$('form').submit(function (e) {\n";
	html += "e.preventDefault();\n";
	html += "var data = new FormData(e.currentTarget);\n";
	html += "document.getElementById(\"submitTRACKER\").disabled=true;\n";
	html += "$.ajax({\n";
	html += "url: '/tracker',\n";
	html += "type: 'POST',\n";
	html += "data: data,\n";
	html += "contentType: false,\n";
	html += "processData: false,\n";
	html += "success: function (data) {\n";
	html += "alert(\"Submited Successfully\");\n";
	html += "},\n";
	html += "error: function (data) {\n";
	html += "alert(\"An error occurred.\");\n";
	html += "}\n";
	html += "});\n";
	html += "});\n";
	html += "</script>\n<script type=\"text/javascript\">\n";
	html += "function openWindowSymbol(sel) {\n";
	html += "var i, l, options = [{\n";
	html += "value: 'first',\n";
	html += "text: 'First'\n";
	html += "}, {\n";
	html += "value: 'second',\n";
	html += "text: 'Second'\n";
	html += "}],\n";
	html += "newWindow = window.open(\"\", null, \"height=400,width=400,status=no,toolbar=no,menubar=no,location=no\");\n";

	int i;

	html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
	html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '/'</th></tr><tr>\");\n";
	for (i = 33; i < 129; i++)
	{
		html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(\"+sel.toString()+\"," + String(i) + ",1);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i) + "-1.png\\\"></td>\");\n";
		if (((i % 16) == 0) && (i < 126))
			html += "newWindow.document.write(\"</tr><tr>\");\n";
	}
	html += "newWindow.document.write(\"</tr></table><br />\");\n";
	html += "newWindow.document.write(\"<table border=\\\"1\\\" align=\\\"center\\\">\");\n";
	html += "newWindow.document.write(\"<tr><th colspan=\\\"16\\\">Table '\\\'</th></tr><tr>\");\n";
	for (i = 33; i < 129; i++)
	{
		html += "newWindow.document.write(\"<td><img onclick=\\\"window.opener.setValue(\"+sel.toString()+\"," + String(i) + ",2);\\\" src=\\\"http://www.dprns.com/symbols/icons/" + String(i, DEC) + "-2.png\\\"></td>\");\n";
		if (((i % 16) == 0) && (i < 126))
			html += "newWindow.document.write(\"</tr><tr>\");\n";
	}
	html += "newWindow.document.write(\"</tr></table>\");\n";

	html += "}\n";

	html += "function setValue(sel,symbol,table) {\n";
	html += "var txtsymbol=document.getElementById('trackerSymbol');\n";
	html += "var txttable=document.getElementById('trackerTable');\n";
	html += "var imgicon=document.getElementById('trackerImgSymbol');\n";
	html += "if(sel==1){\n";
	html += "txtsymbol=document.getElementById('moveSymbol');\n";
	html += "txttable=document.getElementById('moveTable');\n";
	html += "imgicon= document.getElementById('moveImgSymbol');\n";
	html += "}else if(sel==2){\n";
	html += "txtsymbol=document.getElementById('stopSymbol');\n";
	html += "txttable=document.getElementById('stopTable');\n";
	html += "imgicon= document.getElementById('stopImgSymbol');\n";
	html += "}\n";
	html += "txtsymbol.value = String.fromCharCode(symbol);\n";
	html += "if(table==1){\n txttable.value='/';\n";
	html += "}else if(table==2){\n txttable.value='\\\\';\n}\n";
	html += "imgicon.src = \"http://www.dprns.com/symbols/icons/\"+symbol.toString()+'-'+table.toString();\n";
	html += "\n}\n";
	html += "function onSmartCheck() {\n";
	html += "if (document.querySelector('#smartBcnEnable').checked) {\n";
	// Checkbox has been checked
	html += "document.getElementById(\"smartbcnGrp\").disabled=false;\n";
	html += "} else {\n";
	// Checkbox has been unchecked
	html += "document.getElementById(\"smartbcnGrp\").disabled=true;\n";
	html += "}\n}\n";

	html += "</script>\n";

	/************************ tracker Mode **************************/
	html += "<form id='formtracker' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
	// html += "<h2>[TRACKER] Tracker Position Mode</h2>\n";
	html += "<table>\n";
	// html += "<tr>\n";
	// html += "<th width=\"200\"><span><b>Setting</b></span></th>\n";
	// html += "<th><span><b>Value</b></span></th>\n";
	// html += "</tr>\n";
	html += "<th colspan=\"2\"><span><b>[TRACKER] Tracker Position Mode</b></span></th>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>Enable:</b></td>\n";
	String trackerEnFlag = "";
	if (config.trk_en)
		trackerEnFlag = "checked";
	html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"trackerEnable\" value=\"OK\" " + trackerEnFlag + "><span class=\"slider round\"></span></label></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>Station Callsign:</b></td>\n";
	html += "<td style=\"text-align: left;\"><input maxlength=\"7\" size=\"6\" id=\"myCall\" name=\"myCall\" type=\"text\" value=\"" + String(config.trk_mycall) + "\" /></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>Station SSID:</b></td>\n";
	html += "<td style=\"text-align: left;\">\n";
	html += "<select name=\"mySSID\" id=\"mySSID\">\n";
	for (uint8_t ssid = 0; ssid <= 15; ssid++)
	{
		if (config.trk_ssid == ssid)
		{
			html += "<option value=\"" + String(ssid) + "\" selected>" + String(ssid) + "</option>\n";
		}
		else
		{
			html += "<option value=\"" + String(ssid) + "\">" + String(ssid) + "</option>\n";
		}
	}
	html += "</select></td>\n";
	html += "</tr>\n";

	html += "<tr>\n";
	html += "<td align=\"right\"><b>Item/Obj Name:</b></td>\n";
	html += "<td style=\"text-align: left;\"><input maxlength=\"9\" size=\"9\" id=\"trackerObject\" name=\"trackerObject\" type=\"text\" value=\"" + String(config.trk_item) + "\" /><i> *If not used, leave it blank.In use 3-9 charactor</i></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>PATH:</b></td>\n";
	html += "<td style=\"text-align: left;\"><input maxlength=\"72\" size=\"72\" id=\"trackerPath\" name=\"trackerPath\" type=\"text\" value=\"" + String(config.trk_path) + "\" /></td>\n";
	html += "</tr>\n";

	html += "<tr>\n";
	html += "<td align=\"right\"><b>Text Comment:</b></td>\n";
	html += "<td style=\"text-align: left;\"><input maxlength=\"50\" size=\"50\" id=\"trackerComment\" name=\"trackerComment\" type=\"text\" value=\"" + String(config.trk_comment) + "\" /></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>Smart Beacon:</b></td>\n";
	String smartBcnEnFlag = "";
	if (config.trk_smartbeacon)
		smartBcnEnFlag = "checked";
	html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" id=\"smartBcnEnable\" name=\"smartBcnEnable\" onclick=\"onSmartCheck()\" value=\"OK\" " + smartBcnEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Switch use to smart beacon mode</i></label></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\"><b>Compress:</b></td>\n";
	String compressEnFlag = "";
	if (config.trk_compress)
		compressEnFlag = "checked";
	html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"compressEnable\" value=\"OK\" " + compressEnFlag + "><span class=\"slider round\"></span></label><label style=\"vertical-align: bottom;font-size: 8pt;\"><i> *Switch compress packet</i></label></td>\n";
	html += "</tr>\n";
	String trackerPos2RFFlag = "";
	String trackerPos2INETFlag = "";
	if (config.trk_loc2rf)
		trackerPos2RFFlag = "checked";
	if (config.trk_loc2inet)
		trackerPos2INETFlag = "checked";
	html += "<tr><td style=\"text-align: right;\"><b>TX Channel:</b></td><td style=\"text-align: left;\"><input type=\"checkbox\" name=\"trackerPos2RF\" value=\"OK\" " + trackerPos2RFFlag + "/>RF <input type=\"checkbox\" name=\"trackerPos2INET\" value=\"OK\" " + trackerPos2INETFlag + "/>Internet </td></tr>\n";
	String trackerOptBatFlag = "";
	String trackerOptSatFlag = "";
	String trackerOptAltFlag = "";
	String trackerOptCSTFlag = "";
	if (config.trk_bat)
		trackerOptBatFlag = "checked";
	if (config.trk_sat)
		trackerOptSatFlag = "checked";
	if (config.trk_altitude)
		trackerOptAltFlag = "checked";
	if (config.trk_cst)
		trackerOptCSTFlag = "checked";
	html += "<tr><td style=\"text-align: right;\"><b>Option:</b></td><td style=\"text-align: left;\">";
	html += "<input type=\"checkbox\" name=\"trackerOptCST\" value=\"OK\" " + trackerOptCSTFlag + "/>Course/Speed ";
	html += "<input type=\"checkbox\" name=\"trackerOptAlt\" value=\"OK\" " + trackerOptAltFlag + "/>Altitude ";
	html += "<input type=\"checkbox\" name=\"trackerOptBat\" value=\"OK\" " + trackerOptBatFlag + "/>Battery ";
	html += "<input type=\"checkbox\" name=\"trackerOptSat\" value=\"OK\" " + trackerOptSatFlag + "/>Satellite";
	html += "</td></tr>\n";

	html += "<tr>";
	html += "<td align=\"right\"><b>POSITION:</b></td>\n";
	html += "<td align=\"center\">\n";
	html += "<table>";
	html += "<tr><td style=\"text-align: right;\">Interval:</td><td style=\"text-align: left;\"><input min=\"0\" max=\"3600\" step=\"1\" id=\"trackerPosInv\" name=\"trackerPosInv\" type=\"number\" value=\"" + String(config.trk_interval) + "\" />Sec.</label></td></tr>";
	String trackerPosFixFlag = "";
	String trackerPosGPSFlag = "";

	if (config.trk_gps)
		trackerPosGPSFlag = "checked=\"checked\"";
	else
		trackerPosFixFlag = "checked=\"checked\"";

	html += "<tr><td style=\"text-align: right;\">Location:</td><td style=\"text-align: left;\"><input type=\"radio\" name=\"trackerPosSel\" value=\"0\" " + trackerPosFixFlag + "/>Fix <input type=\"radio\" name=\"trackerPosSel\" value=\"1\" " + trackerPosGPSFlag + "/>GPS </td></tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\">Symbol Icon:</td>\n";
	String table = "1";
	if (config.trk_symbol[0] == 47)
		table = "1";
	if (config.trk_symbol[0] == 92)
		table = "2";
	html += "<td style=\"text-align: left;\">Table:<input maxlength=\"1\" size=\"1\" id=\"trackerTable\" name=\"trackerTable\" type=\"text\" value=\"" + String(config.trk_symbol[0]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> Symbol:<input maxlength=\"1\" size=\"1\" id=\"trackerSymbol\" name=\"trackerSymbol\" type=\"text\" value=\"" + String(config.trk_symbol[1]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> <img border=\"1\" style=\"vertical-align: middle;\" id=\"trackerImgSymbol\" onclick=\"openWindowSymbol(0);\" src=\"http://www.dprns.com/symbols/icons/" + String((int)config.trk_symbol[1]) + "-" + table + ".png\"> <i>*Click icon for select symbol</i></td>\n";
	html += "</tr>\n";
	html += "<tr><td style=\"text-align: right;\">Latitude:</td><td style=\"text-align: left;\"><input min=\"-90\" max=\"90\" step=\"0.0001\" id=\"trackerPosLat\" name=\"trackerPosLat\" type=\"number\" value=\"" + String(config.trk_lat, 5) + "\" />degrees (positive for North, negative for South)</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Longitude:</td><td style=\"text-align: left;\"><input min=\"-180\" max=\"180\" step=\"0.0001\" id=\"trackerPosLon\" name=\"trackerPosLon\" type=\"number\" value=\"" + String(config.trk_lon, 5) + "\" />degrees (positive for East, negative for West)</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Altitude:</td><td style=\"text-align: left;\"><input min=\"0\" max=\"10000\" step=\"0.1\" id=\"trackerPosAlt\" name=\"trackerPosAlt\" type=\"number\" value=\"" + String(config.trk_alt, 2) + "\" /> meter. *Value 0 is not send height</td></tr>\n";
	html += "</table></td>";
	html += "</tr>\n";

	html += "<tr>\n";
	html += "<td align=\"right\"><b>Smart Beacon:</b></td>\n";
	html += "<td align=\"center\">\n";
	if (config.trk_smartbeacon)
		html += "<fieldset id=\"smartbcnGrp\">\n";
	else
		html += "<fieldset id=\"smartbcnGrp\" disabled>\n";
	html += "<legend>Smart beacon configuration</legend>\n<table>";
	html += "<tr>\n";
	html += "<td align=\"right\">Move Symbol:</td>\n";
	table = "1";
	if (config.trk_symmove[0] == 47)
		table = "1";
	if (config.trk_symmove[0] == 92)
		table = "2";
	html += "<td style=\"text-align: left;\">Table:<input maxlength=\"1\" size=\"1\" id=\"moveTable\" name=\"moveTable\" type=\"text\" value=\"" + String(config.trk_symmove[0]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> Symbol:<input maxlength=\"1\" size=\"1\" id=\"moveSymbol\" name=\"moveSymbol\" type=\"text\" value=\"" + String(config.trk_symmove[1]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> <img border=\"1\" style=\"vertical-align: middle;\" id=\"moveImgSymbol\" onclick=\"openWindowSymbol(1);\" src=\"http://www.dprns.com/symbols/icons/" + String((int)config.trk_symmove[1]) + "-" + table + ".png\"> <i>*Click icon for select MOVE symbol</i></td>\n";
	html += "</tr>\n";
	html += "<tr>\n";
	html += "<td align=\"right\">Stop Symbol:</td>\n";
	table = "1";
	if (config.trk_symstop[0] == 47)
		table = "1";
	if (config.trk_symstop[0] == 92)
		table = "2";
	html += "<td style=\"text-align: left;\">Table:<input maxlength=\"1\" size=\"1\" id=\"stopTable\" name=\"stopTable\" type=\"text\" value=\"" + String(config.trk_symstop[0]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> Symbol:<input maxlength=\"1\" size=\"1\" id=\"stopSymbol\" name=\"stopSymbol\" type=\"text\" value=\"" + String(config.trk_symstop[1]) + "\" style=\"background-color: rgb(97, 239, 170);\" /> <img border=\"1\" style=\"vertical-align: middle;\" id=\"stopImgSymbol\" onclick=\"openWindowSymbol(2);\" src=\"http://www.dprns.com/symbols/icons/" + String((int)config.trk_symstop[1]) + "-" + table + ".png\"> <i>*Click icon for select STOP symbol</i></td>\n";
	html += "</tr>\n";
	html += "<tr><td style=\"text-align: right;\">High Speed:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"10\" max=\"1000\" step=\"1\" id=\"hspeed\" name=\"hspeed\" type=\"number\" value=\"" + String(config.trk_hspeed) + "\" /> km/h</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Low Speed:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"1\" max=\"250\" step=\"1\" id=\"lspeed\" name=\"lspeed\" type=\"number\" value=\"" + String(config.trk_lspeed) + "\" /> km/h</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Slow Interval:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"60\" max=\"3600\" step=\"1\" id=\"slowInterval\" name=\"slowInterval\" type=\"number\" value=\"" + String(config.trk_slowinterval) + "\" /> Sec.</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Max Interval:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"10\" max=\"255\" step=\"1\" id=\"maxInterval\" name=\"maxInterval\" type=\"number\" value=\"" + String(config.trk_maxinterval) + "\" /> Sec.</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Min Interval:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"1\" max=\"100\" step=\"1\" id=\"minInterval\" name=\"minInterval\" type=\"number\" value=\"" + String(config.trk_mininterval) + "\" /> Sec.</td></tr>\n";
	html += "<tr><td style=\"text-align: right;\">Min Angle:</td><td style=\"text-align: left;\"><input size=\"3\" min=\"1\" max=\"359\" step=\"1\" id=\"minAngle\" name=\"minAngle\" type=\"number\" value=\"" + String(config.trk_minangle) + "\" /> Degree.</td></tr>\n";

	html += "</table></fieldset></tr></table><br />\n";
	html += "<div><button type='submit' id='submitTRACKER'  name=\"commitTRACKER\"> Apply Change </button></div>\n";
	html += "<input type=\"hidden\" name=\"commitTRACKER\"/>\n";
	html += "</form><br />";
	server.send(200, "text/html", html); // send to someones browser when asked
}

void handle_wireless()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	if (server.hasArg("commitWiFiAP"))
	{
		bool wifiAP = false;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "wifiAP")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						wifiAP = true;
					}
				}
			}

			if (server.argName(i) == "wifi_ssidAP")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wifi_ap_ssid, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "wifi_passAP")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wifi_ap_pass, server.arg(i).c_str());
				}
			}
		}
		if (wifiAP)
		{
			config.wifi_mode |= WIFI_AP_FIX;
		}
		else
		{
			config.wifi_mode &= ~WIFI_AP_FIX;
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("commitWiFiClient"))
	{
		bool wifiSTA = false;
		String nameSSID, namePASS;
		for (int n = 0; n < 5; n++)
			config.wifi_sta[n].enable = false;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "wificlient")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						wifiSTA = true;
					}
				}
			}

			for (int n = 0; n < 5; n++)
			{
				nameSSID = "wifiStation" + String(n);
				if (server.argName(i) == nameSSID)
				{
					if (server.arg(i) != "")
					{
						if (String(server.arg(i)) == "OK")
						{
							config.wifi_sta[n].enable = true;
						}
					}
				}
				nameSSID = "wifi_ssid" + String(n);
				if (server.argName(i) == nameSSID)
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_sta[n].wifi_ssid, server.arg(i).c_str());
					}
				}
				namePASS = "wifi_pass" + String(n);
				if (server.argName(i) == namePASS)
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_sta[n].wifi_pass, server.arg(i).c_str());
					}
				}
			}

			if (server.argName(i) == "wifi_pwr")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.wifi_power = server.arg(i).toInt();
				}
			}
		}
		if (wifiSTA)
		{
			config.wifi_mode |= WIFI_STA_FIX;
		}
		else
		{
			config.wifi_mode &= ~WIFI_STA_FIX;
		}
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else if (server.hasArg("commitBluetooth"))
	{
		bool btMaster = false;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "btMaster")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						btMaster = true;
					}
				}
			}

			if (server.argName(i) == "bt_name")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.bt_name, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "bt_uuid")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.bt_uuid, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "bt_uuid_rx")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.bt_uuid_rx, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "bt_uuid_tx")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.bt_uuid_tx, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "bt_mode")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.bt_mode = server.arg(i).toInt();
				}
			}
		}
		config.bt_master = btMaster;
		saveEEPROM();
		String html = "OK";
		server.send(200, "text/html", html);
	}
	else
	{
		String html = "<script type=\"text/javascript\">\n";
		html += "$('form').submit(function (e) {\n";
		html += "e.preventDefault();\n";
		html += "var data = new FormData(e.currentTarget);\n";
		html += "if(e.currentTarget.id===\"formBluetooth\") document.getElementById(\"submitBluetooth\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formWiFiAP\") document.getElementById(\"submitWiFiAP\").disabled=true;\n";
		html += "if(e.currentTarget.id===\"formWiFiClient\") document.getElementById(\"submitWiFiClient\").disabled=true;\n";
		html += "$.ajax({\n";
		html += "url: '/wireless',\n";
		html += "type: 'POST',\n";
		html += "data: data,\n";
		html += "contentType: false,\n";
		html += "processData: false,\n";
		html += "success: function (data) {\n";
		html += "alert(\"Submited Successfully\");\n";
		html += "},\n";
		html += "error: function (data) {\n";
		html += "alert(\"An error occurred.\");\n";
		html += "}\n";
		html += "});\n";
		html += "});\n";
		html += "</script>\n";
		/************************ WiFi AP **************************/
		html += "<form id='formWiFiAP' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		// html += "<h2>WiFi Access Point</h2>\n";
		html += "<table>\n";
		// html += "<tr>\n";
		// html += "<th width=\"200\"><span><b>Setting</b></span></th>\n";
		// html += "<th><span><b>Value</b></span></th>\n";
		// html += "</tr>\n";
		html += "<th colspan=\"2\"><span><b>WiFi Access Point</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\" width=\"120\"><b>Enable:</b></td>\n";
		String wifiAPEnFlag = "";
		if (config.wifi_mode & WIFI_AP_FIX)
			wifiAPEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"wifiAP\" value=\"OK\" " + wifiAPEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>WiFi AP SSID:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"32\" maxlength=\"32\" class=\"form-control\" id=\"wifi_ssidAP\" name=\"wifi_ssidAP\" type=\"text\" value=\"" + String(config.wifi_ap_ssid) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>WiFi AP PASSWORD:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input size=\"63\" maxlength=\"63\" class=\"form-control\" id=\"wifi_passAP\" name=\"wifi_passAP\" type=\"password\" value=\"" + String(config.wifi_ap_pass) + "\" /></td>\n";
		html += "</tr>\n";
		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitWiFiAP'  name=\"commit\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitWiFiAP\"/>\n";
		html += "</form><br />";
		/************************ WiFi Client **************************/
		html += "<br />\n";
		html += "<form id='formWiFiClient' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		html += "<table>\n";
		html += "<th colspan=\"2\"><span><b>WiFi Multi Station</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>WiFi STA Enable:</b></td>\n";
		String wifiClientEnFlag = "";
		if (config.wifi_mode & WIFI_STA_FIX)
			wifiClientEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"wificlient\" value=\"OK\" " + wifiClientEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>WiFi RF Power:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"wifi_pwr\" id=\"wifi_pwr\">\n";
		for (int i = 0; i < 12; i++)
		{
			if (config.wifi_power == wifiPwr[i][0])
				html += "<option value=\"" + String(wifiPwr[i][0], 0) + "\" selected>" + String(wifiPwr[i][1], 1) + " dBm</option>\n";
			else
				html += "<option value=\"" + String(wifiPwr[i][0], 0) + "\" >" + String(wifiPwr[i][1], 1) + " dBm</option>\n";
		}
		html += "</select>\n";
		html += "</td>\n";
		html += "</tr>\n";
		for (int n = 0; n < 5; n++)
		{
			html += "<tr>\n";
			html += "<td align=\"right\"><b>Station #" + String(n + 1) + ":</b></td>\n";
			html += "<td align=\"center\">\n";
			html += "<fieldset id=\"filterDispGrp" + String(n + 1) + "\">\n";
			html += "<legend>WiFi Station #" + String(n + 1) + "</legend>\n<table style=\"text-align:unset;border-width:0px;background:unset\">";
			html += "<tr style=\"background:unset;\">";
			// html += "<tr>\n";
			html += "<td align=\"right\" width=\"120\"><b>Enable:</b></td>\n";
			String wifiClientEnFlag = "";
			if (config.wifi_sta[n].enable)
				wifiClientEnFlag = "checked";
			html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"wifiStation" + String(n) + "\" value=\"OK\" " + wifiClientEnFlag + "><span class=\"slider round\"></span></label></td>\n";
			html += "</tr>\n";
			html += "<tr>\n";
			html += "<td align=\"right\"><b>WiFi SSID:</b></td>\n";
			html += "<td style=\"text-align: left;\"><input size=\"32\" maxlength=\"32\" name=\"wifi_ssid" + String(n) + "\" type=\"text\" value=\"" + String(config.wifi_sta[n].wifi_ssid) + "\" /></td>\n";
			html += "</tr>\n";
			html += "<tr>\n";
			html += "<td align=\"right\"><b>WiFi PASSWORD:</b></td>\n";
			html += "<td style=\"text-align: left;\"><input size=\"63\" maxlength=\"63\" name=\"wifi_pass" + String(n) + "\" type=\"password\" value=\"" + String(config.wifi_sta[n].wifi_pass) + "\" /></td>\n";
			html += "</tr>\n";
			html += "</tr></table></fieldset>\n";
			html += "</td></tr>\n";
		}

		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitWiFiClient'  name=\"commit\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitWiFiClient\"/>\n";
		html += "</form><br />";
		/************************ Bluetooth **************************/
		html += "<br />\n";
		html += "<form id='formBluetooth' method=\"POST\" action='#' enctype='multipart/form-data'>\n";
		// html += "<h2>Bluetooth Master (BLE)</h2>\n";
		html += "<table>\n";
		// html += "<tr>\n";
		// html += "<th width=\"200\"><span><b>Setting</b></span></th>\n";
		// html += "<th><span><b>Value</b></span></th>\n";
		// html += "</tr>\n";
		html += "<th colspan=\"2\"><span><b>Bluetooth Master (BLE)</b></span></th>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>Enable:</b></td>\n";
		String btEnFlag = "";
		if (config.bt_master)
			btEnFlag = "checked";
		html += "<td style=\"text-align: left;\"><label class=\"switch\"><input type=\"checkbox\" name=\"btMaster\" value=\"OK\" " + btEnFlag + "><span class=\"slider round\"></span></label></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>NAME:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"20\" id=\"bt_name\" name=\"bt_name\" type=\"text\" value=\"" + String(config.bt_name) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>UUID:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"37\" size=\"38\" id=\"bt_uuid\" name=\"bt_uuid\" type=\"text\" value=\"" + String(config.bt_uuid) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>UUID RX:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"37\" size=\"38\" id=\"bt_uuid_rx\" name=\"bt_uuid_rx\" type=\"text\" value=\"" + String(config.bt_uuid_rx) + "\" /></td>\n";
		html += "</tr>\n";
		html += "<tr>\n";
		html += "<td align=\"right\"><b>UUID TX:</b></td>\n";
		html += "<td style=\"text-align: left;\"><input maxlength=\"37\" size=\"38\" id=\"bt_uuid_tx\" name=\"bt_uuid_tx\" type=\"text\" value=\"" + String(config.bt_uuid_tx) + "\" /></td>\n";
		html += "</tr>\n";

		html += "<td align=\"right\"><b>MODE:</b></td>\n";
		html += "<td style=\"text-align: left;\">\n";
		html += "<select name=\"bt_mode\" id=\"bt_mode\">\n";
		String btModeOff = "";
		String btModeTNC2 = "";
		String btModeKISS = "";
		if (config.bt_mode == 1)
		{
			btModeTNC2 = "selected";
		}
		else if (config.bt_mode == 2)
		{
			btModeKISS = "selected";
		}
		else
		{
			btModeOff = "selected";
		}
		html += "<option value=\"0\" " + btModeOff + ">NONE</option>\n";
		html += "<option value=\"1\" " + btModeTNC2 + ">TNC2</option>\n";
		html += "<option value=\"2\" " + btModeKISS + ">KISS</option>\n";
		html += "</select>\n";

		html += "<label style=\"font-size: 8pt;text-align: right;\">*See the following for generating UUIDs: <a href=\"https://www.uuidgenerator.net\" target=\"_blank\">https://www.uuidgenerator.net</a></label></td>\n";
		html += "</tr>\n";
		html += "</table><br />\n";
		html += "<div><button type='submit' id='submitBluetooth'  name=\"commit\"> Apply Change </button></div>\n";
		html += "<input type=\"hidden\" name=\"commitBluetooth\"/>\n";
		html += "</form>";
		server.send(200, "text/html", html); // send to someones browser when asked
	}
}

extern bool afskSync;
extern String lastPkgRaw;
extern float dBV;
extern int mVrms;
void handle_realtime()
{
	// char jsonMsg[1000];
	char *jsonMsg;
	time_t timeStamp;
	time(&timeStamp);

	if (afskSync && (lastPkgRaw.length() > 5))
	{
		int input_length = lastPkgRaw.length();
		jsonMsg = (char *)malloc((input_length * 2) + 70);
		char *input_buffer = (char *)malloc(input_length + 2);
		char *output_buffer = (char *)malloc(input_length * 2);
		if (output_buffer)
		{
			lastPkgRaw.toCharArray(input_buffer, lastPkgRaw.length(), 0);
			lastPkgRaw.clear();
			encode_base64((unsigned char *)input_buffer, input_length, (unsigned char *)output_buffer);
			// Serial.println(output_buffer);
			sprintf(jsonMsg, "{\"Active\":\"1\",\"mVrms\":\"%d\",\"RAW\":\"%s\",\"timeStamp\":\"%li\"}", mVrms, output_buffer, timeStamp);
			// Serial.println(jsonMsg);
			free(input_buffer);
			free(output_buffer);
		}
	}
	else
	{
		jsonMsg = (char *)malloc(100);
		if (afskSync)
			sprintf(jsonMsg, "{\"Active\":\"1\",\"mVrms\":\"%d\",\"RAW\":\"REVDT0RFIEZBSUwh\",\"timeStamp\":\"%li\"}", mVrms, timeStamp);
		else
			sprintf(jsonMsg, "{\"Active\":\"0\",\"mVrms\":\"0\",\"RAW\":\"\",\"timeStamp\":\"%li\"}", timeStamp);
	}
	afskSync = false;
	server.send(200, "text/html", String(jsonMsg));

	delay(100);
	free(jsonMsg);
}

// void handle_test()
// {
// 	if (server.hasArg("REBOOT"))
// 	{
// 		esp_restart();
// 	}
// 	if (server.hasArg("sendBeacon"))
// 	{
// 		String tnc2Raw = send_fix_location();
// 		if (config.rf_en)
// 			pkgTxPush(tnc2Raw.c_str(), tnc2Raw.length(), 0);
// 		// APRS_sendTNC2Pkt(tnc2Raw); // Send packet to RF
// 	}
// 	else if (server.hasArg("sendRaw"))
// 	{
// 		for (uint8_t i = 0; i < server.args(); i++)
// 		{
// 			if (server.argName(i) == "raw")
// 			{
// 				if (server.arg(i) != "")
// 				{
// 					String tnc2Raw = server.arg(i);
// 					if (config.rf_en)
// 					{
// 						pkgTxPush(tnc2Raw.c_str(), tnc2Raw.length(), 0);
// 						// APRS_sendTNC2Pkt(server.arg(i)); // Send packet to RF
// 						// Serial.println("Send RAW: " + tnc2Raw);
// 					}
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	// setHTML(6);

// 	webString += "<table>\n";
// 	webString += "<tr><td><form accept-charset=\"UTF-8\" action=\"/test\" class=\"form-horizontal\" id=\"test_form\" method=\"post\">\n";
// 	webString += "<div style=\"margin-left: 20px;\"><input type='submit' class=\"btn btn-danger\" name=\"sendBeacon\" value='SEND BEACON'></div><br />\n";
// 	webString += "<div style=\"margin-left: 20px;\">TNC2 RAW: <input id=\"raw\" name=\"raw\" type=\"text\" size=\"60\" value=\"" + String(config.aprs_mycall) + ">APE32I,WIDE1-1:>Test Status\"/></div>\n";
// 	webString += "<div style=\"margin-left: 20px;\"><input type='submit' class=\"btn btn-primary\" name=\"sendRaw\" value='SEND RAW'></div> <br />\n";
// 	webString += "<div style=\"margin-left: 20px;\"><input type='submit' class=\"btn btn-danger\" name=\"REBOOT\" value='REBOOT'></div><br />\n";
// 	webString += "</form></td></tr>\n";
// 	webString += "<tr><td><hr width=\"80%\" /></td></tr>\n";
// 	webString += "<tr><td><div id=\"vumeter\" style=\"width: 300px; height: 200px; margin: 10px;\"></div></td>\n";
// 	webString += "<tr><td><div style=\"margin: 15px;\">Terminal<br /><textarea id=\"raw_txt\" name=\"raw_txt\" rows=\"25\" cols=\"80\" /></textarea></div></td></tr>\n";
// 	webString += "</table>\n";

// 	webString += "</body></html>\n";
// 	server.send(200, "text/html", webString); // send to someones browser when asked

// 	delay(100);
// 	webString.clear();
// }

void handle_about()
{
	if (!server.authenticate(config.http_username, config.http_password))
	{
		return server.requestAuthentication();
	}
	char strCID[50];
	uint64_t chipid = ESP.getEfuseMac();
	sprintf(strCID, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);

	webString.clear();
	webString += "<table style=\"text-align:unset;border-width:0px;background:unset\"><tr style=\"background:unset;\"><td width=\"49%\" style=\"border:unset;\">";

	webString += "<table>";
	webString += "<th colspan=\"2\"><span><b>System Information</b></span></th>\n";
	// webString += "<tr><th width=\"200\"><span><b>Name</b></span></th><th><span><b>Information</b></span></th></tr>";
	webString += "<tr><td align=\"right\"><b>Hardware Version: </b></td><td align=\"left\"> LILYGO T-TWR Plus </td></tr>";
	webString += "<tr><td align=\"right\"><b>Firmware Version: </b></td><td align=\"left\"> V" + String(VERSION) + String(VERSION_BUILD) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>RF Analog Module: </b></td><td align=\"left\"> MODEL: " + String(RF_TYPE[config.rf_type]) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>ESP32 Model: </b></td><td align=\"left\"> " + String(ESP.getChipModel()) + "</td></tr>";
	webString += "<tr><td align=\"right\"><b>Chip ID: </b></td><td align=\"left\"> " + String(strCID) + "</td></tr>";
	webString += "<tr><td align=\"right\"><b>Revision: </b></td><td align=\"left\"> " + String(ESP.getChipRevision()) + "</td></tr>";
	webString += "<tr><td align=\"right\"><b>Flash: </b></td><td align=\"left\">" + String(ESP.getFlashChipSize() / 1000) + " KByte</td></tr>";
	webString += "<tr><td align=\"right\"><b>PSRAM: </b></td><td align=\"left\">" + String(ESP.getPsramSize() / 1000) + " KByte</td></tr>";
	webString += "</table>";
	webString += "</td><td width=\"2%\" style=\"border:unset;\"></td>";
	webString += "<td width=\"49%\" style=\"border:unset;\">";

	webString += "<table>";
	webString += "<th colspan=\"2\"><span><b>Developer/Support Information</b></span></th>\n";
	// webString += "<tr><th width=\"200\"><span><b>Name</b></span></th><th><span><b>Information</b></span></th></tr>";
	webString += "<tr><td align=\"right\"><b>Name: </b></td><td align=\"left\">Mr.Somkiat Nakhonthai </td></tr>";
	webString += "<tr><td align=\"right\"><b>Callsign: </b></td><td align=\"left\">HS5TQA</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Country: </b></td><td align=\"left\">Bangkok,Thailand</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Github: </b></td><td align=\"left\"><a href=\"https://github.com/nakhonthai\" target=\"_github\">https://github.com/nakhonthai</a></td></tr>";
	webString += "<tr><td align=\"right\"><b>Youtube: </b></td><td align=\"left\"><a href=\"https://www.youtube.com/@HS5TQA\" target=\"_youtube\">https://www.youtube.com/@HS5TQA</a></td></tr>";
	webString += "<tr><td align=\"right\"><b>Facebook: </b></td><td align=\"left\"><a href=\"https://www.facebook.com/atten\" target=\"_facebook\">https://www.facebook.com/atten</a></td></tr>";
	webString += "<tr><td align=\"right\"><b>Chat LINE ID: </b></td><td align=\"left\"><a href=\"https://line.me/ti/p/Pw5MKwm6Vo\" target=\"_line\">nakhonline</a></td></tr>";
	webString += "<tr><td align=\"right\"><b>Sponsors: </b></td><td align=\"left\"><a href=\"https://github.com/sponsors/nakhonthai\" target=\"_sponsor\">https://github.com/sponsors/nakhonthai</a></td></tr>";

	webString += "</table>";
	webString += "</td></tr></table><br />";

	webString += "<table>\n";
	webString += "<th colspan=\"2\"><span><b>WiFi Status</b></span></th>\n";
	webString += "<tr><td align=\"right\"><b>Mode:</b></td>\n";
	webString += "<td align=\"left\">";
	if (config.wifi_mode == WIFI_AP_FIX)
	{
		webString += "AP";
	}
	else if (config.wifi_mode == WIFI_STA_FIX)
	{
		webString += "STA";
	}
	else if (config.wifi_mode == WIFI_AP_STA_FIX)
	{
		webString += "AP+STA";
	}
	else
	{
		webString += "OFF";
	}

	wifi_power_t wpr = WiFi.getTxPower();
	String wifipower = "";
	if (wpr < 8)
	{
		wifipower = "-1 dBm";
	}
	else if (wpr < 21)
	{
		wifipower = "2 dBm";
	}
	else if (wpr < 29)
	{
		wifipower = "5 dBm";
	}
	else if (wpr < 35)
	{
		wifipower = "8.5 dBm";
	}
	else if (wpr < 45)
	{
		wifipower = "11 dBm";
	}
	else if (wpr < 53)
	{
		wifipower = "13 dBm";
	}
	else if (wpr < 61)
	{
		wifipower = "15 dBm";
	}
	else if (wpr < 69)
	{
		wifipower = "17 dBm";
	}
	else if (wpr < 75)
	{
		wifipower = "18.5 dBm";
	}
	else if (wpr < 77)
	{
		wifipower = "19 dBm";
	}
	else if (wpr < 80)
	{
		wifipower = "19.5 dBm";
	}
	else
	{
		wifipower = "20 dBm";
	}

	webString += "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>MAC:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.macAddress()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Channel:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.channel()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>TX Power:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.getTxPower()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>SSID:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.SSID()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Local IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.localIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Gateway IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.gatewayIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>DNS:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.dnsIP().toString() + "</td></tr>\n";
	webString += "</table><br /><br />\n";

	webString += "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form' class=\"form-horizontal\">\n";
	webString += "<table>";
	webString += "<th colspan=\"2\"><span><b>Firmware Update</b></span></th>\n";
	webString += "<tr><td align=\"right\"><b>File:</b></td><td align=\"left\"><input id=\"file\" name=\"update\" type=\"file\" onchange='sub(this)' /></td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Progress:</b></td><td><div id='prgbar'><div id='bar' style=\"width: 0px;\"><label id='prg'></label></div></div></td></tr>\n";
	webString += "</table><br />\n";
	webString += "<div class=\"col-sm-3 col-xs-4\"><input type='submit' class=\"btn btn-danger\" id=\"update_sumbit\" value='Firmware Update'></div>\n";

	webString += "</form>\n";
	webString += "<script>"
				 "function sub(obj){"
				 "var fileName = obj.value.split('\\\\');"
				 "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
				 "};"
				 "$('form').submit(function(e){"
				 "e.preventDefault();"
				 "var form = $('#upload_form')[0];"
				 "var data = new FormData(form);"
				 "document.getElementById('update_sumbit').disabled = true;"
				 "$.ajax({"
				 "url: '/update',"
				 "type: 'POST',"
				 "data: data,"
				 "contentType: false,"
				 "processData:false,"
				 "xhr: function() {"
				 "var xhr = new window.XMLHttpRequest();"
				 "xhr.upload.addEventListener('progress', function(evt) {"
				 "if (evt.lengthComputable) {"
				 "var per = evt.loaded / evt.total;"
				 "$('#prg').html(Math.round(per*100) + '%');"
				 "$('#bar').css('width',Math.round(per*100) + '%');"
				 "}"
				 "}, false);"
				 "return xhr;"
				 "},"
				 "success:function(d, s) {"
				 "alert('Wait for system reboot 10sec') "
				 "},"
				 "error: function (a, b, c) {"
				 "}"
				 "});"
				 "});"
				 "</script>";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked
}

void handle_default()
{
	defaultSetting = true;
	defaultConfig();
	defaultSetting = false;
}

#ifdef SDCARD
void handle_download()
{
	String dataType = "text/plain";
	String path;
	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "FILE")
			{
				path = server.arg(i);
				break;
			}
		}
	}

	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".htm"))
		dataType = "text/html";
	else if (path.endsWith(".csv"))
		dataType = "text/csv";
	else if (path.endsWith(".css"))
		dataType = "text/css";
	else if (path.endsWith(".xml"))
		dataType = "text/xml";
	else if (path.endsWith(".png"))
		dataType = "image/png";
	else if (path.endsWith(".gif"))
		dataType = "image/gif";
	else if (path.endsWith(".jpg"))
		dataType = "image/jpeg";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".svg"))
		dataType = "image/svg+xml";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".js"))
		dataType = "application/javascript";
	else if (path.endsWith(".pdf"))
		dataType = "application/pdf";
	else if (path.endsWith(".zip"))
		dataType = "application/zip";
	else if (path.endsWith(".gz"))
	{
		if (path.startsWith("/gz/htm"))
			dataType = "text/html";
		else if (path.startsWith("/gz/css"))
			dataType = "text/css";
		else if (path.startsWith("/gz/csv"))
			dataType = "text/csv";
		else if (path.startsWith("/gz/xml"))
			dataType = "text/xml";
		else if (path.startsWith("/gz/js"))
			dataType = "application/javascript";
		else if (path.startsWith("/gz/svg"))
			dataType = "image/svg+xml";
		else
			dataType = "application/x-gzip";
	}

	File myFile = SD.open("/" + path, "r");
	if (myFile)
	{
		server.sendHeader("Content-Type", dataType);
		server.sendHeader("Content-Disposition", "attachment; filename=" + path);
		server.sendHeader("Connection", "close");
		server.streamFile(myFile, "application/octet-stream");
		myFile.close();
	}
	delay(100);
}

void handle_delete()
{
	String dataType = "text/plain";
	String path;
	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "FILE")
			{
				path = server.arg(i);
				Serial.println("Deleting file: " + path);
				if (SD.remove("/" + path))
				{
					Serial.println("File deleted");
				}
				else
				{
					Serial.println("Delete failed");
				}
				break;
			}
		}
	}

	handle_storage();
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR : ");
			Serial.print(file.name());
			time_t t = file.getLastWrite();
			struct tm *tmstruct = localtime(&t);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.print(file.size());
			time_t t = file.getLastWrite();
			struct tm *tmstruct = localtime(&t);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
		}
		file = root.openNextFile();
	}
}
#endif

bool webServiceBegin=true;
void webService()
{
	if(webServiceBegin){
		webServiceBegin=false;
	}else{
		return;
	}
	server.close();
	// web client handlers
	server.on("/", setMainPage);
	server.on("/logout", handle_logout);
#ifdef SDCARD
	server.on("/file", handle_storage);
	server.on("/download", handle_download);
	server.on("/delete", handle_delete);
#endif

	server.on("/radio", handle_radio);
	server.on("/vpn", handle_vpn);
	server.on("/default", handle_default);
	server.on("/igate", handle_igate);
	server.on("/digi", handle_digi);
	server.on("/tracker", handle_tracker);
	server.on("/system", handle_system);
	server.on("/symbol", handle_symbol);
	server.on("/wireless", handle_wireless);
	// server.on("/test", handle_test);
	server.on("/realtime", handle_realtime);
	server.on("/about", handle_about);
	server.on("/dashboard", handle_dashboard);
	server.on("/sidebarInfo", handle_sidebar);
	server.on("/sysinfo", handle_sysinfo);
	server.on("/lastHeard", handle_lastHeard);
	server.on("/style.css", handle_css);
	server.on("/jquery-3.7.1.js", handle_jquery);
	/*handling uploading firmware file */
	server.on(
		"/update", HTTP_POST, []()
		{
			server.sendHeader("Connection", "close");
			server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
			ESP.restart(); },
		[]()
		{
			HTTPUpload &upload = server.upload();
			if (upload.status == UPLOAD_FILE_START)
			{
				Serial.printf("Firmware Update FILE: %s\n", upload.filename.c_str());
				if (!Update.begin(UPDATE_SIZE_UNKNOWN))
				{ // start with max available size
					Update.printError(Serial);
					delay(3);
				}
				else
				{
					// wdtDisplayTimer = millis();
					// wdtSensorTimer = millis();
					disableCore0WDT();
					disableCore1WDT();
					disableLoopWDT();

					aprsClient.stop();
					vTaskSuspend(taskAPRSHandle);
					vTaskSuspend(taskTNCHandle);
					vTaskSuspend(taskGpsHandle);
					// vTaskSuspend(taskNetworkHandle);
					// config.igate_en = false;
					// config.rf_en = false;

					delay(3);
				}
			}
			else if (upload.status == UPLOAD_FILE_WRITE)
			{
				/* flashing firmware to ESP*/
				if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
				{
					Update.printError(Serial);
					delay(3);
				}
			}
			else if (upload.status == UPLOAD_FILE_END)
			{
				if (Update.end(true))
				{ // true to set the size to the current progress
					delay(3);
				}
				else
				{
					Update.printError(Serial);
					delay(3);
				}
			}
		});
	server.begin();
}