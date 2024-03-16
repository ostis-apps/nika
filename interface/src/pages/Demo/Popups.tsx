import React from "react";
import { useEffect, useState, useRef } from "react";
import { handleSave } from "@api/sc/agents/helper";

export const CreateQuestionClassPopup = (
    setCreatePopup,
    setCreatePhraseTemplatePopup,
    setForm
) => {
    const questionSystemIdentifierRef = useRef<HTMLInputElement>(null);
    const questionRussianIdentifierRef = useRef<HTMLInputElement>(null);
    const questionWitAiRef = useRef<HTMLInputElement>(null);

    useEffect(() => {
        questionSystemIdentifierRef.current!.value = "concept_message_about_";
        questionRussianIdentifierRef.current!.value = "Сообщение о ";
    }, [questionSystemIdentifierRef, questionRussianIdentifierRef])

    const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        setCreatePopup(false);
        setCreatePhraseTemplatePopup(true);
        setForm(questionSystemIdentifierRef.current?.value + "\n" + questionRussianIdentifierRef.current?.value + "\n" + questionWitAiRef.current?.value);
      };

    const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      setCreatePopup(false);
    };

    const systemIdtfHandleKeyDown = (event) => {
        const inputValue = questionSystemIdentifierRef.current?.value;
        const fixedText = "concept_message_about_";

        if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
        event.preventDefault();
        questionSystemIdentifierRef.current!.value = fixedText;
        }  
    }

    const russianIdtfHandleKeyDown = (event) => {
        const inputValue = questionRussianIdentifierRef.current?.value;
        const fixedText = "Сообщение о ";

        if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
        event.preventDefault();
        questionRussianIdentifierRef.current!.value = fixedText;
        }  
    }

    return (
      <div className="popup">
        <h2>Создание сообщения и шаблонов ответов</h2>
        <button className='close_button' onClick={closeClick}>×</button>
        <div className='numdiv'>
            <button className='p3'>1</button>
            <button className='p4'>2</button>
        </div>
        <div className="form">
            <h4>Системный индетификатор</h4>
            <input
            type="text"
            className="input"
            ref={questionSystemIdentifierRef}
            onKeyDown={systemIdtfHandleKeyDown}
            />
            <h4>Индетификатор на русском языке</h4>
            <input
            type="text"
            className="input"
            ref={questionRussianIdentifierRef}
            onKeyDown={russianIdtfHandleKeyDown}
            />
            <h4>Название интента в Wit.ai</h4>
            <input
            type="text"
            className="input"
            ref={questionWitAiRef}
            placeholder='your_name'
            />
            </div>
            <button className="button-save" onClick={handleClick}>
            Далее
            </button>
        
      </div>
    );
  };

  export const CreatePhraseTemplatePopup = (
    setCreatePhraseTemplatePopup,
    form
  ) => {
    const phraseSystemIdentifierRef = useRef<HTMLInputElement>(null);
    const phraseRussianIdentifierRef = useRef<HTMLInputElement>(null);

    const [chipsValues, setChipsValues] = useState<string[]>([]);
    const [editingIndex, setEditingIndex] = useState(null);
    const handleChipAdd = (value: string) => {
        setChipsValues([...chipsValues, value]);
    };

    useEffect(() => {
        phraseSystemIdentifierRef.current!.value = "concept_phrase_template_";
        phraseRussianIdentifierRef.current!.value = "Шаблон фразы о ";
    }, [phraseSystemIdentifierRef, phraseRussianIdentifierRef])
    
    const handleChipDelete = (index: number) => {
      const newChipsValues = [...chipsValues];
      newChipsValues.splice(index, 1);
      setChipsValues(newChipsValues);
    };

    const handleChipEdit = (index) => {
        setEditingIndex(index);
      };
    
    const handleChipChange = (event, index) => {
      const updatedChipsValues = [...chipsValues];
      updatedChipsValues[index] = event.target.value;
      setChipsValues(updatedChipsValues);
    };

    const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        await handleSave(phraseSystemIdentifierRef, phraseRussianIdentifierRef,
            form, chipsValues);
        setCreatePhraseTemplatePopup(false);
      };

    const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      setCreatePhraseTemplatePopup(false);
    };

    const systemIdtfHandleKeyDown = (event) => {
        const inputValue = phraseSystemIdentifierRef.current?.value;
        const fixedText = "concept_phrase_template_";

        if (inputValue === "concept_phrase_template_" && (event.key === 'Backspace' || event.key === 'Delete')) {
        event.preventDefault();
        phraseSystemIdentifierRef.current!.value = fixedText;
        }  
    }

    const russianIdtfHandleKeyDown = (event) => {
        const inputValue = phraseRussianIdentifierRef.current?.value;
        const fixedText = "Шаблон фразы о ";

        if (inputValue === fixedText && (event.key === 'Backspace' || event.key === 'Delete')) {
        event.preventDefault();
        phraseRussianIdentifierRef.current!.value = fixedText;
        }  
    }

    return (
      <div className="popup">
        <h2>Создание сообщения и шаблонов ответов</h2>
        <button className='close_button' onClick={closeClick}>×</button>
        <div className='numdiv'>
            <button className='p1'>1</button>
            <button className='p2'>2</button>
        </div>
        <div className="form">
            <h4>Системный индетификатор</h4>
            <input
            type="text"
            className="input"
            ref={phraseSystemIdentifierRef}
            onKeyDown={systemIdtfHandleKeyDown}
            />
            <h4>Индетификатор на русском языке</h4>
            <input
            type="text"
            className="input"
            ref={phraseRussianIdentifierRef}
            onKeyDown={russianIdtfHandleKeyDown}
            />
            </div>
            <h2>Шаблоны ответов</h2>
            <div className="chips">
                <input
                    type="text"
                    className="chip-input"
                    placeholder="Шаблон фразы"
                    onKeyDown={(e: React.KeyboardEvent<HTMLInputElement>) => {
                        if (e.key === 'Enter') {
                        handleChipAdd(e.currentTarget.value);
                        e.currentTarget.value = '';
                        }
                    }}
                />
                {chipsValues.map((value, index) => (
                    <div className="chip-div" key={index}>
                    {editingIndex === index ? (
                        <input
                        type="text"
                        className="chip"
                        value={value}
                        onChange={(e) => handleChipChange(e, index)}
                        onBlur={() => setEditingIndex(null)}
                        autoFocus
                        />
                    ) : (
                        <div className="chip" onClick={() => handleChipEdit(index)}>
                        {value}
                        </div>
                    )}
                    <span className="chip-delete" onClick={() => handleChipDelete(index)}>
                        X
                    </span>
                    </div>
                ))}
            </div>
            <button className="button save" onClick={handleClick}>
            Сохранить
            </button>
        
      </div>
    );
  };