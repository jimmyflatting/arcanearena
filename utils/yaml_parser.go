package utils

import (
	"gopkg.in/yaml.v3"
	"io"
	"log"
	"os"
)

type T struct {
	WINDOW_NAME     string `yaml:"WINDOW_NAME"`
	WINDOW_SETTINGS struct {
		MODE string `yaml:"MODE"`
		FPS  int    `yaml:"FPS"`
		SIZE []int  `yaml:"SIZE,flow"`
	} `yaml:"WINDOW_SETTINGS"`

	CONTROLS struct {
		UP    string `yaml:"UP"`
		DOWN  string `yaml:"DOWN"`
		LEFT  string `yaml:"LEFT"`
		RIGHT string `yaml:"RIGHT"`
		A     string `yaml:"A"`
		B     string `yaml:"B"`
		PAUSE string `yaml:"PAUSE"`
		QUIT  string `yaml:"QUIT"`
	} `yaml:"CONTROLS"`
}

func LoadYaml(path string) T {
	// Load the file content
	file, err := os.Open(path)
	if err != nil {
		log.Fatal(err)
	}

	b, err := io.ReadAll(file)
	if err = file.Close(); err != nil {
		log.Fatal(err)
	}

	// Parse the content
	t := T{}

	err = yaml.Unmarshal([]byte(b), &t)
	if err != nil {
		log.Fatalf("error: %v", err)
	}

	return t
}
