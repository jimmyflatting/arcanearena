package game

import (
	"app/ecs"
	"app/utils"
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/sdl"
	"time"
)

var window *sdl.Window
var renderer *sdl.Renderer
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
var imgFlags int = img.INIT_PNG
var ecsManager *ecs.ECSManager

func Init() {
	settings = utils.LoadYaml("data/settings.yaml")

	if err := sdl.Init(sdl.INIT_EVERYTHING); err != nil {
		panic(err)
	}

	window, err = sdl.CreateWindow(settings.WINDOW_NAME, sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED, int32(settings.WINDOW_SETTINGS.SIZE[0]), int32(settings.WINDOW_SETTINGS.SIZE[1]), windowFlags)
	if err != nil {
		panic(err)
	}

	renderer, err = sdl.CreateRenderer(window, -1, sdl.RENDERER_ACCELERATED)
	if err != nil {
		panic(err)
	}

	// initialize SDL_image
	if err := img.Init(imgFlags); err != nil {
		panic(err)
	}

	// initialize ECS
	ecsManager = ecs.NewECSManager()

	// create player entity
	player := ecsManager.CreateEntity()
	ecsManager.AddPositionComponent(player, 100, 100)
	ecsManager.AddSpriteComponent(player, "assets/player.png", 32, 32, 10, renderer)
	ecsManager.AddControlsComponent(player)

	// create some environment entities
	tree := ecsManager.CreateEntity()
	ecsManager.AddPositionComponent(tree, 200, 150)
	ecsManager.AddSpriteComponent(tree, "assets/player.png", 64, 128, 5, renderer)

	// test adding multiple entities
	for i := 0; i < 10; i++ {
		rock := ecsManager.CreateEntity()
		ecsManager.AddPositionComponent(rock, int32(i*50), 300)
		ecsManager.AddSpriteComponent(rock, "assets/player.png", 32, 32, 5, renderer)
	}
}

func Run() {
	defer sdl.Quit()
	defer window.Destroy()

	running := true
	lastTime := time.Now()

	for running {
		currentTime := time.Now()
		deltaTime := currentTime.Sub(lastTime).Seconds()
		lastTime = currentTime

		for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
			running = ecsManager.HandleInput(event, &settings)
		}

		update(deltaTime, &settings)
		render()
		sdl.Delay(16) // roughly 60 FPS
	}
}

func update(deltaTime float64, settings *utils.T) {
	ecsManager.Update(deltaTime, settings)
}

func render() {
	renderer.SetDrawColor(0, 0, 0, 255)
	renderer.Clear()
	ecsManager.Render(renderer)
	renderer.Present()
}
