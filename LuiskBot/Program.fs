open System.Threading.Tasks
open DSharpPlus
open DSharpPlus.EventArgs
open DSharpPlus.Entities


type Coordinate = { X: int; Y: int }


type Command =
    { Prefix: string
      Handler: DiscordClient -> MessageCreateEventArgs -> string -> Async<unit> }


//============================================================================================
let mutable seqNum = 0
let mutable followSeq = 0
let mutable position = { X = 0; Y = 0 }
let mutable offset = { X = 0; Y = 0 }

let mutable imLast = false
let mutable imDirector = false
let mutable imCapitan = false
//============================================================================================

let rowIndex n =
    let expr = 1 + 8 * n
    -1 + (sqrt (float expr) |> int) / 2


// -join // solicitud para unirse
// -granted {ID} {SEQ_NUM} // lo contesta el último de la formación
// -follow {MY_SEQ_NUM} // pregunto a quién sigo y cómo me acomodo
// -follow-ans {ID} {FOLLOW_SEQ_NUM} {OFFSET_X} {OFFSET_Y} // respuesta del director
// -move {POS_X} {POS_Y} // el director ordena al capitan moverse
// -pos {SEQ_NUM} {POS_X} {POS_Y} // anuncio que me muevo a una nueva posición
// -regroup // se resetea la formación



let displace (coord: Coordinate) (offset: Coordinate) =
    { X = coord.X + offset.X
      Y = coord.Y + offset.Y }



let statusHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        do!
            DiscordMessageBuilder()
                .WithContent(
                    $"```==================================\n\
                                  ID:         {client.CurrentUser.Id}\n\
                                  Sequence:   {seqNum}\n\
                                  Follow:     {followSeq}\n\
                                  Position:   ({position.X}, {position.Y})\n\
                                  Offset:     ({offset.X}, {offset.Y})\n\
                                  Last:       {imLast}\n\
                                  Director:   {imDirector}\n\
                                  Capitan:    {imCapitan}\n\
                                  ==================================```"
                )
                .SendAsync(eventArgs.Channel)
            |> Async.AwaitTask
            |> Async.Ignore
    }


let lastHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) = async { imLast <- not imLast }

let directorHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) = async { imDirector <- not imDirector }

let capitanHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) = async { imCapitan <- not imCapitan }

//=================================================================================================================================
let echoHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        do!
            DiscordMessageBuilder()
                .WithContent($"{message}")
                .SendAsync(eventArgs.Channel)
            |> Async.AwaitTask
            |> Async.Ignore
    }



let joinHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        if imLast then
            imLast <- false

            do!
                DiscordMessageBuilder()
                    .WithContent($"-granted {eventArgs.Author.Id} {seqNum}")
                    .SendAsync(eventArgs.Channel)
                |> Async.AwaitTask
                |> Async.Ignore
    }


let grantedHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        let messageArgs = message.Split(' ')

        if (messageArgs.[0] |> uint64) = client.CurrentUser.Id then
            seqNum <- ((messageArgs.[1] |> int) + 1)
            imLast <- true

            do!
                DiscordMessageBuilder()
                    .WithContent($"-follow {seqNum}")
                    .SendAsync(eventArgs.Channel)
                |> Async.AwaitTask
                |> Async.Ignore
    }


let followHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        if imDirector then
            do!
                DiscordMessageBuilder()
                    .WithContent($"-follow-ans {eventArgs.Author.Id} {seqNum} {offset.X} {offset.Y}")
                    .SendAsync(eventArgs.Channel)
                |> Async.AwaitTask
                |> Async.Ignore
    }


let followAnsHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        let messageArgs = message.Split(' ')

        if (messageArgs.[0] |> uint64) = client.CurrentUser.Id then
            let intArgs = [ for i in messageArgs -> i |> int ]
            followSeq <- intArgs.[1]
            offset <- { X = intArgs.[2]; Y = intArgs.[3] }
    }


let moveHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        if imCapitan then
            let messageArgs =
                [ for i in message.Split(' ') -> i |> int ]

            position <-
                { X = messageArgs.[0]
                  Y = messageArgs.[1] }

            do!
                DiscordMessageBuilder()
                    .WithContent($"-pos {seqNum} {position.X} {position.Y}")
                    .SendAsync(eventArgs.Channel)
                |> Async.AwaitTask
                |> Async.Ignore
    }


let posHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        let messageArgs =
         [ for i in message.Split(' ') -> i |> int ]
        if followSeq = messageArgs.[0] && not imCapitan then
            position <- displace {X = messageArgs.[1]; Y = messageArgs.[2]} offset

            do!
                DiscordMessageBuilder()
                    .WithContent($"-pos {seqNum} {position.X} {position.Y}")
                    .SendAsync(eventArgs.Channel)
                |> Async.AwaitTask
                |> Async.Ignore
    }


let regroupHandler (client: DiscordClient) (eventArgs: MessageCreateEventArgs) (message: string) =
    async {
        seqNum <- 0
        followSeq <- 0
        imLast <- false
        position <- { X = 0; Y = 0 }
        offset <- { X = 0; Y = 0 }
    }



//=================================================================================================================================
let commands =
    [ { Prefix = "-status"
        Handler = statusHandler }
      { Prefix = "-echo"
        Handler = echoHandler }
      { Prefix = "-last"
        Handler = lastHandler }
      { Prefix = "-setDirector"
        Handler = directorHandler }
      { Prefix = "-setCapitan"
        Handler = capitanHandler }
      { Prefix = "-join"
        Handler = joinHandler }
      { Prefix = "-granted"
        Handler = grantedHandler }
      { Prefix = "-follow-ans"
        Handler = followAnsHandler }
      { Prefix = "-follow"
        Handler = followHandler }
      { Prefix = "-move"
        Handler = moveHandler }
      { Prefix = "-pos"
        Handler = posHandler }
      { Prefix = "-regroup"
        Handler = regroupHandler } ]




//=================================================================================================================================
let dispatchCommand client eventArgs (message: string) =
    let maybeCommand =
        commands
        |> List.tryFind (fun command -> message.StartsWith command.Prefix)

    match maybeCommand with
    | Some command ->
        let messageWithNoPrefix =
            match command.Prefix.Length with
            | 0 -> message
            | k -> message.[k + 1..]

        command.Handler client eventArgs messageWithNoPrefix
    | None -> async { do printfn "Failed to find a command handler for message: %s" message }


let onMessageCreated (client: DiscordClient) (eventArgs: MessageCreateEventArgs) =
    dispatchCommand client eventArgs eventArgs.Message.Content
    |> Async.StartAsTask
    :> Task




//=================================================================================================================================
[<EntryPoint>]
let main _ =

    let discord =
        new DiscordClient(DiscordConfiguration(Token = "ODU1MDA2MTU1MzY1Mjg1ODk5.YMsMiA.6Moihqa18MyNC-skX_N7euwZ7_Y", TokenType = TokenType.Bot))

    discord.add_MessageCreated (fun sender eventArgs -> onMessageCreated sender eventArgs)

    async {
        do! discord.ConnectAsync() |> Async.AwaitTask
        do! Task.Delay(-1) |> Async.AwaitTask
    }
    |> Async.RunSynchronously

    0
