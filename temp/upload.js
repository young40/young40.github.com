'use strict';

var list = [
    {id: 333, name: "aaaaaaa"},
    {id: 234, name: "bbbbbb"},
    {id: 113, name: "ccccc"},
    {id: 224, name: "eeeddd"},

];
var s = 'Hello';

function GetData(id) {
    for(var i=0; i<list.length; i++)
    {
        if (list[i].id == id)
        {
            return list[i];
        }
    }

    return null;
}

window.GetData = GetData;
