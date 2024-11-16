export interface CLIENT_MESSAGE {
    OPERATION: string
}

export interface INIT_MESSAGE extends CLIENT_MESSAGE {
    OPERATION: "INIT"
    FEN?: string
    engine_color: "white" | "black"
}