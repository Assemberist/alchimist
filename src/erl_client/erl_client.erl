-module(erl_client).
-export([initial/1, receiver/1, reader/1, one_request/0]).

-define(requests,[
    "I command to kick all guests",                 %1
    "I command to name he as",                      %2
    "I command to kick this gay",                   %3
    "I command this gay get out from my server",    %4
    "I command kick all gays",                      %5
    "I command all gays get out from my server",    %6
    "I command to name that gay as",                %7
    "I command all go fuck yourself",               %8
    "I command to execute exterminatus on this server. This resourse should be burned in a cleansing fire",
    "I command to kick this guest",                 %10

    "List sessions",        %11
    "List libs",            %12
    "Add me",               %13
    "Create session",       %14
    "Summ",                 %15
    "Who are there",        %16
    "I`m out",              %17
    "Now i`m",              %18
    "Who connected"         %19
]).

initial(Sock) ->
    %gen_tcp:send(Sock, construct_request([18,"sosymba"])),
    gen_tcp:send(Sock, construct_request([14,"/home/assa/sources/alchimist/debug"])),
    %gen_tcp:send(Sock, construct_request([13,"6"])),
    ok.

one_request() ->
    {ok, Sock} = gen_tcp:connect(loopback, 5000, [{active, false}], 2000),
    {ok, Request} = io:read("Put your request>"),
    gen_tcp:send(Sock, Request),
    {ok, Answer} = gen_tcp:recv(Sock, 0),
    io:format("~n~s~n", [Answer]),
    one_request().

receiver(Sock) ->
    {ok, Answer} = gen_tcp:recv(Sock, 0),
    io:format("~n~s~n", [Answer]),
    receiver(Sock).

reader(Sock) ->
    {ok, Request} = io:read("Put your request>"),
    gen_tcp:send(Sock, construct_request(Request)),
    reader(Sock).

construct_request([Num | Tail]) ->
    Args = [lists:nth(Num, ?requests) | Tail],
    [lists:foldr(fun string:concat/2, [], lists:join(":", Args)) | [0]].
    