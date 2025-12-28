# Rasa Classifier

## NLU Module Configuration

1. **config.yml**

    This file configures the NLU pipeline for Russian and English languages. It supplements the standard classifier defined in the file with custom components that the user can add for various purposes.

2. **data/nlu.yml**

    A file for defining custom intents and providing examples for them. It also allows for annotating entities within messages. This is the primary dataset used for model training.

3. **replacements.yml**

    A file used by one of the custom components for word replacements. For example, if a message contains the word "Бресте" (inflected form), the resulting entity extraction will return "Брест" (base form).

4. **components**

    A directory where users can place their custom components. To enable a component, it must subsequently be specified in `config.yml`.

## Building the Classifier

To interact with the classifier, navigate to its directory:

```sh
cd problem-solver/cxx/message-classification-module/rasa_classifier_model/
```

### Training the Classifier

To start the model training process, run the Docker service:

```sh
docker compose build rasa-train
docker compose up rasa-train
```

### Testing the Classifier

Testing requires an existing model in the models directory within the classifier folder. To start the testing process, run the Docker service:

```sh
docker compose build rasa-test
docker compose up rasa-test
```

Testing the classifier involves entering questions into the console. The output will be the classifier's response in JSON format.

## Running the Classifier

The classifier must be launched from the project's root directory. To run the classifier, a trained model must exist in the models directory within the classifier folder. If this directory contains two or more models, the most recent one will be selected. To launch the classifier, run the Docker service:

```sh
docker compose up rasa
```