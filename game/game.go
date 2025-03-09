package game

import (
	"app/utils"
	"fmt"
	"github.com/veandco/go-sdl2/sdl"
)

var window *sdl.Window
var err error
var settings utils.T
var running bool
var windowFlags uint32 = sdl.WINDOW_SHOWN | func() uint32 {
	switch settings.WINDOW_SETTINGS.MODE {
	case "WINDOWED":
		return 0
	case "FULLSCREEN":
		return sdl.WINDOW_FULLSCREEN
	case "BORDERLESS":
		return sdl.WINDOW_BORDERLESS
	}
	return 0
}()

func Init() {
	// Initialize settings data
	settings = utils.LoadYaml("data/settings.yaml")

	// Initialize SDL2
	if err := sdl.Init(sdl.INIT_EVERYTHING); err != nil {
		panic(err)
	}

	window, err = sdl.CreateWindow(settings.WINDOW_NAME, sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED, int32(settings.WINDOW_SETTINGS.SIZE[0]), int32(settings.WINDOW_SETTINGS.SIZE[1]), windowFlags)
	if err != nil {
		panic(err)
	}
}

func Run() {
	defer sdl.Quit()
	defer window.Destroy()

	running = true
	for running {
		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			switch event.(type) {
			case *sdl.QuitEvent:
				println("Quit")
				running = false
				break
			default:
				handlePlayerInput(&event)
			}
		}
		update()
		render()
		sdl.Delay(16)
	}
}

func handlePlayerInput(e *sdl.Event) {
	switch e := (*e).(type) {
	case *sdl.KeyboardEvent:
		if e.Type == sdl.KEYDOWN {
			switch e.Keysym.Sym {
			case sdl.GetKeyFromName("v"):
				fmt.Println("V pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.UP):
				fmt.Println("Up pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.LEFT):
				fmt.Println("Left pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.DOWN):
				fmt.Println("Down pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.RIGHT):
				fmt.Println("Right pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.A):
				fmt.Println("A pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.B):
				fmt.Println("B pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.PAUSE):
				fmt.Println("Pause pressed")
			case sdl.GetKeyFromName(settings.CONTROLS.QUIT):
				fmt.Println("Quit pressed")
				running = false
				break
			}
		}
	}
}

func update() {
	// fmt.Println("Updating game state")
}

func render() {
	// fmt.Println("Rendering game state")
}
