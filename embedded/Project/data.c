//plotterarm operations
OPER drawing[]={
/**
{'A',9000,4500}, //turn base to 90 degree, axis to 45 degree
{'a',1000,2000}, //turn 10 and 20 degree respectively
{'M',100,200}, //move to absolute coordinate (100,200)
{'m',30,40}, //move relatively (30,40)
{'L',300,400}, //pen down, draw a line to absolute coordinate (300,400)
{'l',50,60}, //draw a line relatively (50,60)
{'B',500,600}, //specify the control point of bezier curve at (500,600)
{'C',300,400}, //draw a degree2 bezier curve to (300,400)
{'b',50,50}, //control point relative to the current point of the plotter
{'c',100,150}, //bezier curve relative to the current point of the plotter
{'^','Q',0} //quit, indicating the last operation
*/
/**
{'M',2200,1900}, //upper left range
{'M',1000,2400}, //upper right range
{'M',2500,0}, //lower left range
{'M',1100,0}, //lower right range
{'M',2000,1900},{'l',100,0},{'l',100,0},{'l',100,0},{'l',100,0},{'m',-400,100}, //out of range example
*/

/* //a 4x4 grid
{'M',2000,0},
{'l',400,0},{'m',-400,100},
{'l',400,0},{'m',-400,100},
{'l',400,0},{'m',-400,100},
{'l',100,0},{'l',100,0},{'l',100,0},{'l',100,0},{'m',-400,100},
{'l',100,0},{'l',100,0},{'l',100,0},{'l',100,0},
{'m',-400,-400},
{'l',0,400},{'m',100,-400},
{'l',0,400},{'m',100,-400},
{'l',0,400},{'m',100,-400},
{'l',0,400},{'m',100,-400},
{'l',0,400},
{'m',-400,-400}, //a cross inside
{'l',400,400},{'m',-400,0},{'l',400,-400}, */

/* //a circle in a square
{'M',2000,500},
{'l',400,0},{'l',0,-400},{'l',-400,0},{'l',0,400},
{'m',200,0},
{'b',170,-30},{'c',200,-200},
{'b',-30,-170},{'c',-200,-200},
{'b',-170,30},{'c',-200,200},
{'b',30,170},{'c',200,200}, */

/* //pi
{'M',1468,625},{'B',1557,620},{'C',1683,659},
{'B',1895,711},{'C',1930,563},
{'M',1794,583},{'B',1858,593},{'C',1930,563},
{'M',1794,583},{'L',1816,341},
{'B',1915,247},{'C',1811,244},
{'B',1744,257},{'C',1740,341},
{'L',1722,570},
{'M',1621,565},{'L',1722,570},
{'M',1621,565},{'B',1663,242},{'C',1532,242},
{'B',1448,247},{'C',1443,326},
{'M',1562,563},{'B',1584,235},{'C',1443,326},
{'M',1562,563},{'B',1465,543},{'C',1468,625}, */

/* //pi 2
{'M',1459,609},{'B',1552,604},{'C',1683,645},
{'B',1905,699},{'C',1941,545},
{'M',1799,565},{'B',1866,575},{'C',1941,545},
{'M',1799,565},{'L',1823,312},
{'B',1926,214},{'C',1817,212},
{'B',1748,225},{'C',1743,312},
{'L',1725,552},
{'M',1619,547},{'L',1725,552},
{'M',1619,547},{'B',1663,209},{'C',1526,209},
{'B',1438,214},{'C',1433,297},
{'M',1557,545},{'B',1580,201},{'C',1433,297},
{'M',1557,545},{'B',1456,524},{'C',1459,609}, */

/* //pi 2 shifted
{'M',1975,996},{'B',2068,991},{'C',2199,1032},
{'B',2421,1086},{'C',2457,932},
{'M',2315,952},{'B',2383,963},{'C',2457,932},
{'M',2315,952},{'L',2339,699},
{'B',2442,601},{'C',2334,599},
{'B',2264,612},{'C',2259,699},
{'L',2241,939},
{'M',2135,934},{'L',2241,939},
{'M',2135,934},{'B',2179,596},{'C',2042,596},
{'B',1954,601},{'C',1949,684},
{'M',2073,932},{'B',2096,588},{'C',1949,684},
{'M',2073,932},{'B',1972,911},{'C',1975,996}, */

/* //cpeg
{'M',1444,1347},{'B',1372,1545},{'C',1539,1608},
{'B',1832,1648},{'C',1768,1319},
{'B',1729,1244},{'C',1677,1263},
{'B',1653,1291},{'C',1685,1323},
{'B',1733,1481},{'C',1606,1505},
{'B',1499,1485},{'C',1527,1382},
{'B',1634,1335},{'C',1539,1275},
{'B',1428,1267},{'C',1444,1347},
{'M',1471,1152},{'B',1360,1026},{'C',1463,895},
{'B',1582,820},{'C',1653,899},
{'B',1705,990},{'C',1657,1085},
{'B',1800,1057},{'C',1792,1129},
{'B',1800,1216},{'C',1471,1152},
{'M',1519,1065},{'B',1471,974},{'C',1547,950},
{'B',1626,950},{'C',1582,1069},
{'L',1519,1065},
{'M',1539,444},{'B',1408,483},{'C',1416,610},
{'B',1428,780},{'C',1586,772},
{'B',1752,760},{'C',1756,594},
{'B',1749,451},{'C',1685,436},
{'B',1661,392},{'C',1614,440},
{'B',1578,507},{'C',1669,519},
{'B',1717,646},{'C',1642,677},
{'B',1618,574},{'C',1539,444},
{'M',1586,697},{'B',1539,713},{'C',1503,673},
{'B',1471,594},{'C',1523,566},
{'L',1586,697},
{'M',1448,71},{'B',1349,214},{'C',1440,317},
{'B',1558,420},{'C',1634,325},
{'B',1689,222},{'C',1653,131},
{'B',1717,115},{'C',1741,253},
{'B',1689,341},{'C',1760,356},
{'B',1836,337},{'C',1828,178},
{'B',1804,-16},{'C',1448,71},
{'M',1511,147},{'B',1440,257},{'C',1543,285},
{'B',1614,273},{'C',1562,139},
{'L',1511,147}, */

/* //cc
{'M',1405,1310},{'B',1315,1560},{'C',1525,1640},
{'B',1895,1690},{'C',1815,1275},
{'B',1765,1180},{'C',1700,1205},
{'B',1670,1240},{'C',1710,1280},
{'B',1770,1480},{'C',1610,1510},
{'B',1475,1485},{'C',1510,1355},
{'B',1645,1295},{'C',1525,1220},
{'B',1385,1210},{'C',1405,1310},
{'M',1405,815},{'B',1315,1060},{'C',1525,1140},
{'B',1895,1190},{'C',1815,775},
{'B',1765,680},{'C',1700,705},
{'B',1670,740},{'C',1710,780},
{'B',1770,980},{'C',1610,1010},
{'B',1475,985},{'C',1510,855},
{'B',1645,795},{'C',1525,720},
{'B',1385,710},{'C',1405,815},
{'M',1660,1685},{'B',1925,1625},{'C',1860,1360},
{'B',1815,1150},{'C',1860,990},
{'B',1900,700},{'C',1760,660},
{'B',1655,635},{'C',1620,740},
{'B',1540,640},{'C',1425,680},
{'B',1345,720},{'C',1375,815},
{'B',1265,1085},{'C',1495,1175},
{'B',1335,1175},{'C',1365,1315},
{'B',1275,1595},{'C',1510,1680},
{'B',1590,1700},{'C',1660,1685},
{'M',1405,405},{'B',1340,495},{'C',1435,515},
{'B',1540,495},{'C',1605,395},
{'B',1530,350},{'C',1505,305},
{'B',1465,260},{'C',1420,260},
{'B',1330,305},{'C',1405,405}, */


 //hugo's automaton drawing
{'M',1382,1838},{'B',1002,1853},{'C',923,1362},
{'M',903,1184},{'B',784,535},{'C',1442,507},
{'B',1576,455},{'C',1683,507},
{'B',1952,515},{'C',2119,764},
{'B',2313,1073},{'C',2162,1402},
{'M',1782,1735},{'B',1826,1743},{'C',1881,1691},
{'B',1945,1691},{'C',1976,1644},
{'B',2075,1616},{'C',2135,1473},
{'M',1461,2091},{'B',1394,1850},{'C',1362,1667},
{'M',1842,2036},{'B',1754,1739},{'C',1687,1580},
{'B',1588,1327},{'C',1422,1283},
{'B',1347,1374},{'C',1362,1667},
{'M',1461,2091},{'B',1481,2222},{'C',1663,2202},
{'B',1853,2154},{'C',1842,2036},
{'M',1584,1236},{'B',1636,1295},{'C',1699,1236},
{'B',1723,1208},{'C',1703,1168},
{'B',1766,1057},{'C',1659,1010},
{'B',1560,958},{'C',1545,1042},
{'M',1620,1180},{'B',1703,1160},{'C',1687,1057},
{'M',1616,1022},{'B',1671,1034},{'C',1679,1077},
{'M',1430,1097},{'B',1481,1109},{'C',1529,1081},
{'B',1354,1109},{'C',1291,1093},
{'B',871,1006},{'C',1331,752},
{'M',1414,808},{'B',1370,848},{'C',1331,879},
{'B',1267,982},{'C',1386,1030},
{'B',1430,1034},{'C',1477,1050},
{'B',1533,1065},{'C',1453,970},
{'B',1414,903},{'C',1426,804},
{'M',1901,1164},{'B',1766,1188},{'C',1762,1117},
{'B',1762,1081},{'C',1743,1038},
{'B',1727,943},{'C',1826,927},
{'B',1861,927},{'C',1885,895},
{'B',1972,895},{'C',2036,943},
{'M',1921,1164},{'B',1976,1188},{'C',2000,1156},
{'B',2131,1050},{'C',2028,970},
{'M',1909,1149},{'B',1980,1172},{'C',1988,1141},
{'M',1909,1149},{'B',1774,1160},{'C',1786,1081},
{'B',1747,1002},{'C',1786,978},
{'B',1822,966},{'C',1861,943},
{'B',1889,899},{'C',1929,923},
{'B',2067,931},{'C',2059,1026},
{'B',2048,1117},{'C',1988,1141},
{'M',1374,1057},{'B',1240,1038},{'C',1267,931},
{'B',1291,844},{'C',1386,812},
{'M',1497,2119},{'B',1485,2028},{'C',1612,1988},
{'B',1782,1952},{'C',1814,2040},
{'B',1802,2139},{'C',1663,2170},
{'B',1537,2186},{'C',1497,2119},
{'M',1814,2040},{'B',1802,2139},{'C',1663,2170},
{'B',1537,2186},{'C',1497,2119},
{'M',1525,2107},{'B',1529,2051},{'C',1604,2020},
{'B',1731,1984},{'C',1794,2028},
{'B',1802,2111},{'C',1679,2150},
{'B',1537,2170},{'C',1525,2107},
{'M',1588,2099},{'B',1580,2059},{'C',1640,2040},
{'B',1723,2028},{'C',1711,2103},
{'B',1624,2162},{'C',1588,2099},
{'B',1624,2040},{'C',1703,2071},
{'B',1675,2139},{'C',1612,2099},
{'M',1469,2071},{'B',1517,1937},{'C',1699,1941},
{'B',1774,1941},{'C',1826,2008},
{'M',1426,1921},{'B',1529,1774},{'C',1762,1818},
{'M',1394,1782},{'B',1489,1644},{'C',1711,1675},
{'M',1370,1644},{'B',1477,1529},{'C',1663,1549},
{'M',1370,1501},{'B',1450,1434},{'C',1604,1438},
{'M',1382,1394},{'B',1430,1343},{'C',1533,1358},
{'M',1489,1267},{'B',1552,1311},{'C',1588,1283},
{'B',1659,1283},{'C',1703,1303},
{'M',1636,1283},{'B',1735,1267},{'C',1778,1283},
{'B',1850,1335},{'C',2044,1291},
{'M',1584,966},{'B',1644,962},{'C',1628,931},
{'B',1628,855},{'C',1655,911},
{'B',1703,863},{'C',1743,808},
{'B',1687,792},{'C',1655,848},
{'M',1711,1406},{'B',1952,1422},{'C',2107,1450},
{'B',2325,1529},{'C',2317,1473},
{'B',2269,1414},{'C',2174,1426},
{'B',2051,1430},{'C',1909,1398},
{'M',2115,1434},{'B',2329,1537},{'C',2325,1453},
{'B',2309,1418},{'C',2198,1430},
{'M',1857,1366},{'B',1996,1362},{'C',2131,1335},
{'M',1667,1184},{'L',1675,1204},
{'M',1473,756},{'B',1457,677},{'C',1620,665},
{'M',1442,756},{'B',1414,693},{'C',1509,665},
{'M',1426,764},{'B',1382,721},{'C',1446,653},
{'M',1378,729},{'B',1358,685},{'C',1426,626},
{'M',1743,903},{'B',1754,828},{'C',1881,832},
{'M',1885,867},{'B',1949,840},{'C',1992,871},
{'M',1873,1236},{'B',1937,1263},{'C',2020,1232},
{'M',1925,1200},{'B',1976,1236},{'C',2032,1196},
{'M',1568,1370},{'B',1620,1370},{'C',1663,1382},
{'B',1754,1402},{'C',1719,1354},
{'M',1691,1366},{'B',1735,1386},{'C',1703,1350},
{'M',1719,1362},{'B',1679,1335},{'C',1612,1339},
{'M',1707,1350},{'B',1651,1331},{'C',1564,1323},
{'B',1525,1319},{'C',1481,1299},
{'M',974,523},{'B',1022,495},{'C',1030,428},
{'B',1097,376},{'C',1105,341},
{'B',1141,265},{'C',1125,218},
{'M',1145,190},{'B',1208,281},{'C',1319,226},
{'B',1501,317},{'C',1517,99},
{'M',1010,194},{'B',1141,226},{'C',1180,91},
{'B',1232,91},{'C',1251,40},
{'M',1743,28},{'B',1818,250},{'C',1945,261},
{'B',1933,305},{'C',1956,356},
{'B',1996,408},{'C',2020,424},
{'M',2170,67},{'B',2127,174},{'C',2242,182},
{'B',2210,384},{'C',2321,404},
{'M',867,2154},{'B',883,2016},{'C',828,2020},
{'B',741,2008},{'C',772,1937},
{'B',832,1881},{'C',768,1794},
{'M',2020,2333},{'B',2020,2277},{'C',2055,2269},
{'B',2044,2154},{'C',2178,2162},
{'B',2210,2091},{'C',2277,2099},
{'M',2040,1826},{'B',2063,1830},{'C',2075,1814},
{'B',2095,1663},{'C',2170,1683},
{'M',1909,499},{'B',1893,471},{'C',1937,475},
{'B',1992,495},{'C',1988,455},
{'M',1703,408},{'B',1735,440},{'C',1786,436},
{'B',1822,448},{'C',1846,471},
{'M',2004,1497},{'B',1988,1465},{'C',1968,1465},
{'B',1937,1457},{'C',1929,1442},
{'M',2115,950},{'B',2083,1065},{'C',2178,1152},
{'M',2091,1109},{'B',2115,998},{'C',2048,919},
{'M',1794,1469},{'B',1715,1450},{'C',1600,1378},
{'M',1750,1481},{'B',1695,1461},{'C',1644,1418},
{'M',1513,990},{'B',1418,950},{'C',1450,840},
{'M',1236,646},{'B',1287,638},{'C',1307,689},
{'M',1608,333},{'B',1687,333},{'C',1679,261},
{'B',1711,321},{'C',1770,273},
{'B',1683,349},{'C',1770,404},
{'M',1592,333},{'B',1683,360},{'C',1655,420},
{'B',1687,349},{'C',1770,404},
{'M',1089,455},{'B',1137,463},{'C',1125,519},
{'B',1168,479},{'C',1200,511},
{'B',1176,459},{'C',1204,440},
{'B',1156,448},{'C',1137,392},
{'B',1137,459},{'C',1093,451},
{'M',816,1743},{'B',863,1770},{'C',863,1727},
{'B',887,1758},{'C',943,1731},
{'B',879,1762},{'C',899,1802},
{'B',875,1790},{'C',855,1810},
{'B',863,1770},{'C',832,1766},
{'M',1576,162},{'B',1588,257},{'C',1703,174},
{'M',1921,1976},{'B',1945,1956},{'C',1949,1913},
{'B',1976,1949},{'C',2051,1853},
{'B',2004,1968},{'C',2051,1988},
{'B',1964,1960},{'C',1988,2020},
{'B',1941,1964},{'C',1881,1988},
{'M',2182,2032},{'B',2115,1984},{'C',2253,1897},
{'M',2210,1802},{'B',2257,1834},{'C',2293,1723},
{'M',2218,1580},{'B',2313,1600},{'C',2297,1537},
{'M',1513,388},{'B',1556,380},{'C',1529,349},
{'B',1509,305},{'C',1545,313},
{'M',1279,2059},{'B',1275,2008},{'C',1335,2032},
{'B',1366,2067},{'C',1335,2103},
{'B',1263,2119},{'C',1240,2059},
{'M',1046,1754},{'B',1073,1750},{'C',1061,1794},
{'B',1034,1810},{'C',1022,1754},
{'M',1172,1620},{'B',1050,1651},{'C',1164,1679},
{'B',1212,1663},{'C',1152,1640},
{'M',1034,1505},{'B',1006,1513},{'C',1030,1485},
{'B',1057,1485},{'C',1053,1505},
{'M',947,1335},{'B',931,1295},{'C',895,1307},
{'B',899,1358},{'C',947,1339},
{'M',923,1291},{'B',903,1271},{'C',903,1240},
{'M',1002,950},{'B',1030,907},{'C',1050,986},
{'M',1267,701},{'B',1212,717},{'C',1251,661},
{'M',1362,1343},{'L',820,1703},
{'M',1232,1386},{'L',895,1517},
{'M',1279,1339},{'L',840,1489},
{'M',1125,1651},{'L',887,1897},
{'M',1327,1347},{'L',903,1608},
{'M',1299,1422},{'L',1105,1584},
{'M',1295,1438},{'L',931,1762},
{'M',1311,1446},{'L',1176,1600},
{'M',1311,1473},{'L',1212,1612},
{'M',1331,1473},{'L',958,2186},
{'M',1287,1529},{'L',1022,2040},
{'M',1323,1327},{'L',1220,1327},
{'M',1283,1311},{'L',859,1271},
{'M',1180,1275},{'L',871,1244},
{'M',1065,1251},{'L',895,1200},
{'M',1117,1228},{'L',879,1133},
{'M',1283,1267},{'L',1121,1180},
{'M',1069,1141},{'L',911,1006},
{'M',923,1133},{'L',867,1077},
{'M',1109,1347},{'L',883,1374},
{'M',1350,1267},{'L',911,947},
{'M',1220,1137},{'L',899,780},
{'M',1350,1248},{'L',919,713},
{'M',1200,1018},{'L',903,598},
{'M',907,1679},{'L',848,1723},
{'M',950,1798},{'L',895,1850},
{'M',950,1873},{'L',891,1937},
{'M',986,1905},{'L',891,2016},
{'M',1010,1830},{'L',935,1913},
{'M',1196,1699},{'L',998,1933},
{'M',1061,1941},{'L',939,2143},
{'M',978,1956},{'L',935,2016},
{'M',820,1632},{'L',756,1671},
{'M',832,1588},{'L',800,1608},
{'M',1053,1909},{'L',954,2055},
{'M',1446,966},{'B',1394,974},{'C',1382,935},
{'B',1374,895},{'C',1422,887},
{'M',1493,2004},{'L',1461,1897},
{'M',1612,1929},{'L',1580,1834},
{'M',1501,1826},{'L',1469,1719},
{'M',1426,1739},{'L',1394,1632},
{'M',1529,1671},{'L',1497,1572},
{'M',1430,1584},{'L',1410,1493},
{'M',1529,1549},{'L',1501,1457},
{'M',1374,1485},{'L',1390,1398},
{'M',1537,1430},{'L',1430,1303},
{'M',1564,1406},{'L',1473,1315},
{'M',1723,1901},{'L',1560,1453},
{'M',1636,1798},{'L',1608,1683},
{'M',1766,1905},{'L',1663,1620},
{'M',1703,1786},{'L',1679,1711},
{'M',1620,1533},{'L',1588,1461},
{'M',1350,1810},{'L',1438,2214},
{'M',1311,1584},{'L',1200,2210},
{'M',1327,1865},{'L',1271,2218},
{'M',1937,1703},{'L',2147,2024},
{'M',1141,582},{'L',1061,459},
{'M',1283,527},{'L',1208,313},
{'M',1592,451},{'L',1604,257},
{'M',1857,527},{'L',1913,364},
{'M',1937,606},{'L',2012,467},
{'L',2059,539},
{'M',2154,784},{'L',2198,749},
{'M',1790,1766},{'L',1941,2186},
{'M',2111,1564},{'L',2289,1663},
{'M',1073,1331},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1053,1323},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1976,1133},{'B',1869,1141},{'C',1857,1069},
{'B',1830,1002},{'C',1873,947},
{'M',2012,1085},{'B',1949,1097},{'C',1893,1065},
{'M',1972,978},{'B',1901,939},{'C',1846,974},
{'M',1861,1014},{'L',1917,1022},
{'M',1865,994},{'L',1937,1002},
{'M',1885,1034},{'L',1873,970},
{'M',1897,1010},{'L',1893,970},
{'M',2024,1050},{'L',2020,1006},
{'M',1786,1014},{'L',1830,1133},
{'M',1794,998},{'L',1834,1125},
{'M',1814,1121},{'L',1830,1030},
{'M',1933,1046},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1909,1109},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2004,1117},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2198,1101},{'L',2190,1061},
{'M',2182,1002},{'L',2162,954},
{'M',2214,1101},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2226,1089},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2226,1065},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2218,1046},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2206,1030},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2198,1002},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2198,986},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2194,966},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2186,950},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1501,875},{'L',1465,808},
{'M',1525,859},{'L',1485,796},
{'M',1529,780},{'L',1501,756},
{'M',1560,808},{'L',1533,729},
{'M',1568,844},{'L',1600,855},
{'M',1620,792},{'L',1659,796},
{'M',1600,772},{'L',1651,756},
{'M',1632,792},{'L',1604,729},
{'M',1667,725},{'L',1659,685},
{'M',1390,618},{'L',1370,578},
{'M',1418,598},{'L',1402,550},
{'M',1180,729},{'L',1168,705},
{'M',1184,693},{'L',1176,657},
{'M',1184,824},{'L',1129,796},
{'M',1180,800},{'L',1121,772},
{'M',1133,808},{'L',1117,756},
{'M',1149,804},{'L',1133,745},
{'M',1053,816},{'L',1077,804},
{'M',1370,1038},{'L',1382,1077},
{'M',1327,1018},{'L',1315,1061},
{'M',1295,970},{'L',1251,962},
{'M',1323,867},{'L',1291,824},
{'M',1366,836},{'L',1374,784},
{'M',1038,618},{'L',994,558},
{'M',1057,598},{'L',1014,531},
{'M',1073,586},{'L',1026,503},
{'M',1097,574},{'L',1046,475},
{'M',1224,531},{'L',1164,380},
{'M',1248,523},{'L',1224,467},
{'M',1307,515},{'L',1244,317},
{'M',1339,499},{'L',1287,293},
{'M',1402,487},{'L',1370,364},
{'M',1450,483},{'L',1398,301},
{'M',1473,463},{'L',1438,289},
{'M',1497,459},{'L',1473,368},
{'M',1513,455},{'L',1473,289},
{'M',1525,455},{'L',1481,289},
{'M',1533,455},{'L',1501,285},
{'M',1545,455},{'L',1529,281},
{'M',1556,455},{'L',1556,265},
{'M',1568,432},{'L',1576,273},
{'M',1616,459},{'L',1616,372},
{'M',1640,467},{'L',1632,396},
{'M',1679,487},{'L',1671,436},
{'M',1695,491},{'L',1687,408},
{'M',1715,495},{'L',1711,420},
{'M',1762,499},{'L',1770,345},
{'M',1778,495},{'L',1802,305},
{'L',1834,297},
{'L',1885,321},
{'M',1850,499},{'L',1885,380},
{'M',1988,590},{'L',2028,511},
{'M',2048,653},{'L',2075,618},
{'M',2087,697},{'L',2127,646},
{'M',2202,891},{'L',2246,887},
{'M',2174,816},{'L',2210,788},
{'M',2095,1580},{'L',2285,1691},
{'M',2075,1604},{'L',2242,1735},
{'M',2044,1628},{'L',2238,1790},
{'M',2028,1655},{'L',2226,1842},
{'M',2000,1667},{'L',2198,1865},
{'M',1980,1683},{'L',2154,1873},
{'M',1968,1695},{'L',2154,1925},
{'M',1909,1711},{'L',1992,1861},
{'M',1885,1715},{'L',1952,1873},
{'M',1834,1758},{'L',1909,1937},
{'M',1810,1762},{'L',1960,2147},
{'M',1743,1917},{'L',1719,1842},
{'M',1798,1937},{'L',1766,1869},
{'M',1691,1651},{'L',1628,1493},
{'M',1529,1402},{'L',1465,1327},
{'M',1584,1418},{'L',1517,1343},
{'M',1549,1434},{'L',1481,1339},
{'L',1450,1350},
{'M',1442,1457},{'L',1418,1374},
{'M',1442,1390},{'L',1414,1319},
{'M',1402,1271},{'L',1358,1228},
{'M',1410,1271},{'L',1394,1196},
{'M',1430,1267},{'L',1461,1200},
{'M',1457,1283},{'L',1493,1224},
{'L',1576,1176},
{'M',1450,1192},{'L',1430,1152},
{'M',1450,1172},{'L',1442,1149},
{'M',1453,1061},{'L',1497,1085},
{'M',1430,1057},{'L',1457,1093},
{'M',1350,1038},{'L',1350,1073},
{'M',1311,1018},{'L',1283,1030},
{'M',1291,998},{'L',1259,1006},
{'M',1295,939},{'L',1259,923},
{'M',1291,899},{'L',1267,879},
{'M',1311,891},{'L',1271,851},
{'L',1323,812},
{'M',1347,851},{'L',1339,800},
{'M',1354,848},{'L',1354,784},
{'M',1319,776},{'L',1358,752},
{'M',1137,1038},{'L',1117,935},
{'M',1168,1050},{'L',1145,911},
{'M',1109,982},{'L',1152,939},
{'M',1133,923},{'L',1176,891},
{'M',1196,851},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1212,844},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1687,1485},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1679,1477},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1663,1457},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1659,1442},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1370,1782},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1358,1770},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1350,1750},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1347,1711},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'L',1339,1659},
{'M',1339,1648},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1335,1636},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1350,1624},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1339,1616},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1347,1588},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1339,1513},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1307,1750},{'L',1327,1762},
{'M',1327,1750},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1743,1549},{'L',1786,1580},
{'M',1782,1556},{'L',1774,1529},
{'L',1810,1533},
{'M',1747,1564},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1758,1576},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1754,1560},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1743,1636},{'L',1778,1671},
{'L',1798,1659},
{'M',1798,1648},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1814,1655},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1814,1648},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1830,1648},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1857,1636},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1826,1604},{'L',1869,1584},
{'M',1945,1572},{'L',2000,1549},
{'M',1925,1608},{'L',1972,1608},
{'M',1980,1588},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2012,1592},{'L',2024,1580},
{'M',2024,1556},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2028,1537},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2040,1513},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1897,1533},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1917,1533},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1929,1525},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1905,1513},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1933,1501},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1913,1517},{'L',1921,1450},
{'M',2087,851},{'L',2051,828},
{'M',2004,788},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1992,780},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2004,776},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2020,764},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1766,634},{'L',1731,558},
{'M',1786,626},{'L',1750,578},
{'M',1802,618},{'L',1762,558},
{'M',1818,614},{'L',1774,562},
{'M',1735,622},{'L',1798,606},
{'M',1735,594},{'L',1810,594},
{'M',1834,586},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1850,590},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1695,562},{'L',1644,562},
{'M',1529,523},{'L',1616,535},
{'M',1533,574},{'L',1517,531},
{'M',1568,570},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1580,570},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1592,574},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1715,729},{'L',1727,752},
{'M',1790,749},{'L',1723,709},
{'M',1719,685},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1731,685},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1691,685},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1770,685},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1834,646},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1838,630},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1826,638},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1865,626},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1834,630},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1842,618},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1861,665},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1842,689},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1869,693},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1814,689},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1327,586},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1307,586},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1212,638},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1786,816},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1798,796},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1968,630},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1984,646},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',2004,650},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1988,665},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1996,669},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1834,796},{'L',1865,796},
{'M',1168,1339},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1192,1263},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1192,1251},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1073,1109},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1061,1097},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',978,1042},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',978,1030},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',994,1030},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',994,1065},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1172,1968},{'L',1125,2150},
{'M',1354,1913},{'L',1315,2198},
{'M',1374,2016},{'L',1362,2154},
{'M',1933,2016},{'L',1984,2139},
{'M',2000,2067},{'L',2016,2103},
{'M',2028,2020},{'L',2055,2059},
{'L',2087,2032},
{'M',1850,2079},{'L',1933,2285},
{'M',1822,2139},{'L',1869,2261},
{'M',1723,1917},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1727,1933},{'L',1818,2210},
{'M',1778,2190},{'L',1830,2305},
{'M',1750,2190},{'L',1774,2253},
{'M',1719,2194},{'L',1747,2289},
{'M',1683,2214},{'L',1695,2257},
{'M',1624,2226},{'L',1632,2281},
{'M',1529,2214},{'L',1533,2253},
{'M',1485,2202},{'L',1493,2238},
{'M',1426,2044},{'L',1453,2198},
{'M',1576,1038},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1580,1034},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1842,1137},{'L',1802,998},
{'M',1806,1129},{'L',1794,1105},
{'M',1810,1121},{'L',1774,1026},
{'M',1818,1125},{'L',1782,1030},
{'M',1822,1109},{'L',1778,994},
{'M',1850,1137},{'L',1802,990},
{'M',1806,986},{'L',1853,1141},
{'L',1814,986},
{'M',1822,974},{'L',1834,1030},
{'M',1838,1010},{'L',1830,978},
{'M',1834,966},{'L',1842,1002},
{'M',1877,1141},{'L',1857,1105},
{'M',1881,1141},{'L',1865,1109},
{'M',1885,1137},{'L',1850,1097},
{'M',1897,1141},{'L',1877,1125},
{'M',1913,1149},{'L',1881,1125},
{'M',1945,1152},{'L',1921,1133},
{'M',1960,1152},{'L',1945,1141},
{'M',1972,1149},{'L',1956,1141},
{'M',1937,1149},{'L',1897,1129},
{'M',1826,1133},{'L',1842,1050},
{'L',1850,1089},
{'M',1794,1077},{'L',1834,970},
{'M',1782,1042},{'L',1802,986},
{'M',1778,1002},{'L',1806,978},
{'M',1778,1053},{'L',1786,990},
{'M',1533,1255},{'L',1572,1279},
{'M',1564,1263},{'L',1620,1291},
{'M',1715,1236},{'L',1778,1244},
{'M',1731,1204},{'L',1782,1188},
{'M',1727,1172},{'L',1735,1164},
{'M',1719,1152},{'L',1731,1152},
{'M',1727,1129},{'L',1735,1117},
{'M',1731,1097},{'L',1735,1093},
{'M',1731,1081},{'L',1739,1077},
{'M',1719,1050},{'L',1731,1050},
{'M',1711,1038},{'L',1719,1042},
{'M',1703,1034},{'L',1711,1030},
{'M',1691,1026},{'L',1703,1018},
{'L',1687,1010},
{'M',1663,1014},{'L',1667,1006},
{'M',1624,990},{'L',1620,978},
{'M',1600,990},{'L',1596,982},
{'M',1572,990},{'L',1576,986},
{'L',1572,974},
{'M',1727,1224},{'L',1778,1224},
{'M',1739,1204},{'L',1786,1240},
{'M',1667,1184},{'L',1679,1204},
{'L',1691,1180},
{'M',1699,1255},{'L',1758,1263},
{'M',1509,978},{'L',1493,978},
{'M',1505,966},{'L',1481,970},
{'M',1493,958},{'L',1481,954},
{'M',1473,943},{'L',1485,939},
{'M',1465,903},{'L',1477,899},
{'M',1457,891},{'L',1485,883},
{'M',1461,879},{'L',1473,871},
{'M',1457,863},{'L',1469,855},
{'L',1461,840},
{'M',1517,1022},{'L',1489,1006},
{'M',1513,1061},{'L',1513,1053},
{'L',1517,1042},
{'M',1410,935},{'L',1394,935},
{'M',1422,943},{'L',1402,958},
{'M',1406,927},{'L',1394,923},
{'M',1410,919},{'L',1402,907},
{'M',1414,915},{'L',1406,903},
{'M',1426,907},{'L',1418,915},
{'M',1414,919},{'L',1414,939},
{'L',1434,947},
{'L',1422,962},
{'M',1438,950},{'L',1446,966},
{'M',1442,958},{'L',1473,927},
{'M',1366,725},{'L',1370,713},
{'M',1358,705},{'L',1370,693},
{'M',1354,685},{'L',1390,657},
{'M',1358,677},{'L',1366,661},
{'M',1287,919},{'L',1271,911},
{'M',1295,1010},{'L',1283,1018},
{'M',1291,958},{'L',1275,954},
{'M',1402,1057},{'L',1410,1073},
{'M',1145,1038},{'L',1133,966},
{'M',1156,1050},{'L',1145,990},
{'M',1109,1002},{'L',1137,970},
{'M',1109,1018},{'L',1133,994},
{'M',1121,1026},{'L',1141,1010},
{'L',1188,859},
{'M',1180,1061},{'L',1176,1038},
{'M',1192,1065},{'L',1188,1053},
{'M',1200,1061},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1208,1073},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1208,1061},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1220,1073},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1236,1077},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1275,1085},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1299,1101},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1164,887},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1180,879},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},{'l',0,0},
{'M',1774,1711},{'L',1838,1719},
{'M',1794,1691},{'L',1853,1707},
{'L',1834,1711},
{'M',1838,1687},{'L',1877,1683},
{'M',1865,1679},{'L',1889,1679},
{'M',1822,1695},{'L',1857,1695},
{'M',1386,1335},{'L',1347,1323},
{'M',1390,1299},{'L',1350,1287},
{'M',1394,1255},{'L',1386,1240},
{'M',1418,1255},{'L',1414,1228},
{'M',1426,1255},{'L',1426,1248},
{'M',2202,871},{'L',2222,867},
{'M',2190,851},{'L',2206,848},
{'M',2178,836},{'L',2190,828},
{'M',2143,756},{'L',2154,737},
{'M',2103,713},{'L',2111,697},
{'M',2024,630},{'L',2036,618},
{'M',2040,642},{'L',2048,622},
{'M',2075,677},{'L',2095,650},
{'M',2242,1109},{'L',2269,1109},
{'M',2242,1089},{'L',2265,1089},
{'M',2250,1069},{'L',2261,1065},
{'M',2111,1390},{'L',2150,1398},
{'M',2079,1473},{'L',2115,1497},
{'M',2051,1493},{'L',2103,1505},
{'M',2265,1453},{'L',2170,1442},
{'M',2174,1489},{'L',2313,1549},
{'M',970,1885},{'L',911,1960},
{'M',950,1850},{'L',867,1937},
{'M',1018,1945},{'L',962,2024},
{'M',970,1857},{'L',859,2000},
{'M',911,1798},{'L',927,1786},
{'M',935,1711},{'L',895,1739},
{'M',931,1695},{'L',875,1731},
{'M',863,1659},{'L',796,1699},
{'M',2202,570},{'B',2150,511},{'C',2119,578},
{'B',2162,638},{'C',2257,586},
{'B',2305,523},{'C',2115,487},
{'L',2083,507},
{'L',2182,515},
{'L',2246,507},
{'L',2178,487},
{'L',2190,467},
{'L',2214,459},
{'L',2218,436},
{'L',2166,412},
{'L',2178,396},
{'L',2162,388},
{'L',2178,364},
{'L',2178,337},
{'L',2115,341},
{'L',2147,317},
{'L',2154,285},
{'L',2115,257},
{'L',2083,242},
{'L',2131,222},
{'L',2147,234},
{'L',2150,253},
{'M',2087,356},{'L',2087,349},
{'M',1988,253},{'B',1921,218},{'C',1952,95},
{'B',2012,178},{'C',2095,162},
{'L',2111,150},
{'L',2115,127},
{'L',2099,111}, //*/

{'^','Q',0}
};
