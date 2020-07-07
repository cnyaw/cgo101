//
// cgo101.js
// cgo101 web.
//
// 2020/7/5 Waync created.
//

var CARD_WIDTH = 71;
var CARD_HEIGHT = 96;

var c0 = document.getElementById('c0');
var ctx2d = c0.getContext("2d");

var imgBkgnd = document.getElementById("imgBkgnd");
var imgCard = document.getElementById("imgCard");
var imgCover = document.getElementById("imgCover");
var imgDeck = document.getElementById("imgDeck");

c0.onmousedown = function(e) {
  var off = getEventOffset(e);
  var x = off.x, y = off.y;
  if (Module.ccall('cMouseDown', 'number', ['number', 'number'], [x, y])) {
    drawGame();
  }
}

c0.onmouseup = function(e) {
  var off = getEventOffset(e);
  var x = off.x, y = off.y;
  if (Module.ccall('cMouseUp', 'number', ['number', 'number'], [x, y])) {
    drawGame();
  }
}

c0.onmousemove = function(e) {
  var off = getEventOffset(e);
  var x = off.x, y = off.y;
  if (Module.ccall('cMouseMove', 'number', ['number', 'number'], [x, y])) {
    drawGame();
  }
}

function drawGame() {
  c0.width = window.innerWidth;
  c0.height = window.innerHeight;
  setClientRect(c0.width, c0.height);
  var n = Module.ccall('cDrawGame', 'number', []);
  for (var i = 0; i < n; i++) {
    switch (getDrawGameData(i))
    {
    case 0:                             // Erase bkgnd.
      var pattern = ctx2d.createPattern(imgBkgnd, "repeat");
      ctx2d.fillStyle = pattern;
      ctx2d.rect(0, 0, c0.width, c0.height);
      ctx2d.fill();
      break;
    case 1:                             // Draw empty card.
      var x = getDrawGameData(i + 1);
      var y = getDrawGameData(i + 2);
      i += 2;
      ctx2d.drawImage(imgDeck, 0, 0, imgDeck.width, imgDeck.height, x, y, imgDeck.width, imgDeck.height);
      break;
    case 2:                             // Draw cover.
      var x = getDrawGameData(i + 1);
      var y = getDrawGameData(i + 2);
      i += 2;
      ctx2d.drawImage(imgCover, 0, 0, imgCover.width, imgCover.height, x, y, imgCover.width, imgCover.height);
      break;
    case 3:                             // Draw card.
      var x = getDrawGameData(i + 1);
      var y = getDrawGameData(i + 2);
      var card = getDrawGameData(i + 3);
      i += 3;
      var suit = Math.floor(card / 13);
      var num = card % 13;
      ctx2d.drawImage(imgCard, num * CARD_WIDTH, suit * CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT, x, y, CARD_WIDTH, CARD_HEIGHT);
      break;
    }
  }
}

function getDrawGameData(index) {
  return Module.ccall('cGetDrawGameData', 'number', ['number'], [index]);
}

function getEventOffset(e) {
  if (e.offsetX) {
    return {x:e.offsetX, y:e.offsetY};
  }
  var el = e.target;
  var offset = {x:0, y:0};
  while (el.offsetParent) {
    offset.x += el.offsetLeft;
    offset.y += el.offsetTop;
    el = el.offsetParent;
  }
  offset.x = e.pageX - offset.x;
  offset.y = e.pageY - offset.y;
  return offset;
}

function newGame(sel) {
  Module.ccall('cNewGame', 'number', ['number'], [sel]);
  drawGame();
}

function selGame() {
  var x = document.getElementById("GameMenu").value - 1;
  if (sel != x) {
    sel = x;
    newGame(x);
  }
}

function setClientRect(w, h) {
  Module.ccall('cSetClientRect', 'number', ['number', 'number'], [w, h]);
}