#!/usr/bin/env escript

main(_) ->
    {ok, Sock} = gen_tcp:connect(loopback, 5000, [{active, false}], 2000),
    spawn(erl_client, receiver, [Sock]),
    erl_client:reader(Sock).
    %erl_client:one_request().
