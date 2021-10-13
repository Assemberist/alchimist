-module(erl_client).
-export([receiver/1, reader/1, one_request/0]).

one_request() ->
    {ok, Sock} = gen_tcp:connect(loopback, 5000, [{active, false}], 2000),
    {ok, Request} = io:read("Put your request>"),
    gen_tcp:send(Sock, Request),
    {ok, Answer} = gen_tcp:recv(Sock, 0),
    io:format("\n~s\n", [Answer]),
    one_request().

receiver(Sock) ->
    {ok, Answer} = gen_tcp:recv(Sock, 0),
    io:format("\n~s\n", [Answer]),
    receiver(Sock).

reader(Sock) ->
    {ok, Request} = io:read("Put your request>"),
    gen_tcp:send(Sock, Request),
    reader(Sock).