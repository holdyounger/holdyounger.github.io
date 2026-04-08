package httptest

import (
	"fmt"
	"net/http"
)

func indexHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "hello  world")
}
func htmlHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/html")
	html := `<!doctype  html>  
    <META  http-equiv="Content-Type"  content="text/html"  charset="utf-8">    
    <html  lang="zhCN">  
            <head>   
                    <title>Golang</title>  
                    <meta  name="viewport"  content="width=device-width,  initial-scale=1.0,  maximum-scale=1.0,  user-scalable=0;"  />   
            </head>     
            <body>        
            <div  id="app">Welcome!</div>       
            </body>   
    </html>`
	fmt.Fprintf(w, html)

	fmt.Println("Get html request")
}

func Start() {
	mux := http.NewServeMux()
	mux.Handle("/", http.HandlerFunc(indexHandler))
	mux.HandleFunc("/welcome", htmlHandler)
	http.ListenAndServe("127.0.0.1:8001", mux)

	fmt.Println("Get html request")
}
