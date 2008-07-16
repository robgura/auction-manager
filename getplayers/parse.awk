#!/bin/awk -f

# data source
# wget -O players http://sports.yahoo.com/nfl/players?type=lastname&query=A
# this url seems to get all the players, not just last name A

BEGIN{
    line = 0;
}

{

    if(index($0, "ysprow1") != 0 || index($0, "ysprow2") != 0)
    {
        line = 1;
    }

    if(line == 2)
    {
        value = getId();
        printf("%s#", value);

        value = getName();
        printf("%s#", value);
    }
    else if(line == 3)
    {
        value = getPos();
        printf("%s#", value);
    }
    else if(line == 4)
    {
        value = getName();
        printf("%s#", value);
    }
    else if(line == 5)
    {
        printf("\n");
        line = 0;
    }

    if(line > 0)
    {
        line++;
    }

}

function getName()
{
    l = length($0);
    link = index($0, "<a");

    cut1 = substr($0, link + 1);

    return betweenAngle(cut1);
}

function betweenAngle(value)
{
    startAng = index(value, ">");
    endAng = index(substr(value, startAng), "<") + startAng - 1;

    return substr(value, startAng + 1, endAng - startAng - 1);
}

function getPos()
{
    return betweenAngle($0);
}

function getId()
{
    player = index($0, "players/");
    cut1 = substr($0, player);
    idslash = index(cut1, "/");
    cut2 = substr(cut1, idslash + 1);
    quote = index(cut2, "\"");
    return substr(cut2, 1, quote - 1);
}

