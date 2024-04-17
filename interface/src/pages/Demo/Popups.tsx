import React from "react";
import { useEffect, useState, useRef } from "react";
import { handleSave, userClose, handleSaveClassInstance, findAnyInKb, findRelationsInKb, handleSaveToCreateClass, handleRelationInstance, handleSaveClassInstanceWithRelations } from "@api/sc/agents/helper";
import { AutoComplete } from "antd";
import { FormInput } from "./styled";

  export const CreateClassInstancePopup = (
    setCreateClassInstancePopup,
    setCreateRelationToEntityPopup,
    setFirstForm
  ) => {
    const classInstanceSystemIdentifierRef = useRef<HTMLInputElement>(null);
    const classInstanceRussianIdentifierRef = useRef<HTMLInputElement>(null);
    const classInstanceNoteRef = useRef<HTMLInputElement>(null);
    const [selectedOption, setSelectedOption] = useState('');
    const [inputValue, setInputValue] = useState('concept_');
    const [availableClasses, setAvailableClasses] = useState<{label: string; value: string;}[]>([]);

    const onSelect = (data, option) => {
      setSelectedOption(option);
      setInputValue(option.label);
    };

    const onChange = (data, option) => {
      setInputValue(data);
      setSelectedOption(option);
    };

    useEffect(() => {
      findAnyInKb(setAvailableClasses, "concept_");
    }, [])

    const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        await handleSaveClassInstance(
          classInstanceSystemIdentifierRef,
          classInstanceRussianIdentifierRef,
          classInstanceNoteRef,
          inputValue
        );
        setCreateClassInstancePopup(false);
      };

    const handleAddRelationClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
        setFirstForm([
          classInstanceSystemIdentifierRef.current?.value,
          classInstanceRussianIdentifierRef.current?.value,
          classInstanceNoteRef.current?.value,
          inputValue
        ]);
        setCreateClassInstancePopup(false);
        setCreateRelationToEntityPopup(true);
      };

    const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      await userClose();
      setCreateClassInstancePopup(false);
    };


    return (
      <div className="popup">
        <h2>Создание экземпляра класса</h2>
        <button className='close_button' onClick={closeClick}>×</button>
        <div className="form">
            <h4>Системный идентификатор</h4>
            <input
            type="text"
            className="input"
            placeholder="Системный идентификатор вашего экземпляра"
            ref={classInstanceSystemIdentifierRef}
            />
            <h4>Идентификатор на русском языке</h4>
            <input
            type="text"
            className="input"
            placeholder="Название вашего экземпляра"
            ref={classInstanceRussianIdentifierRef}
            />
            <h4>Примечание(определение)</h4>
            <input
            type="text"
            className="input"
            placeholder="Описание вашего экземпляра"
            ref={classInstanceNoteRef}
            />
            <h4>Класс</h4>
            <AutoComplete
              value={inputValue}
              options={availableClasses}
              autoFocus={true}
              style={{ width: 480 }}
              filterOption={(inputValue, option) =>
                option?.label.toUpperCase().indexOf(inputValue.toUpperCase()) !== -1
              }
              onSelect={onSelect}
              onChange={onChange}
              dropdownStyle={{zIndex: 10000}}
            >
              <FormInput />
            </AutoComplete>
            <button className="button next" onClick={handleAddRelationClick}>Добавить отношения</button>
            </div>
            <button className="button save" onClick={handleClick}>
            Сохранить
            </button>
        
      </div>
    );
  };

  export const AddRelationToEntityPopup = (setCreateRelationToEntityPopup, firstForm: string[]) => {
    const [selectedEntityOption, setSelectedEntityOption] = useState('');
    const [entity, setEntity] = useState('concept_');
    const [selectedRelationOption, setSelectedRelationOption] = useState('');
    const [relation, setRelation] = useState('nrel_');
    const [availableEntity, setAvailableEntity] = useState<{label: string; value: string;}[]>([]);
    const [availableRelations, setAvailableRelations] = useState<{label: string; value: string;}[]>([]);
    const [form, setForm] = useState<{entity: string; relation: string;}[]>([]);

    const onSelectEntity = (data, option) => {
      setSelectedEntityOption(option);
      setEntity(option.label);
    };

    const onChangeEntity = (data, option) => {
      setEntity(data);
      setSelectedEntityOption(option);
    };

    const onSelectRelation = (data, option) => {
      setSelectedRelationOption(option);
      setRelation(option.label);
    };

    const onChangeRelation = (data, option) => {
      setRelation(data);
      setSelectedRelationOption(option);
    };

    useEffect(() => {
      findAnyInKb(setAvailableEntity, "concept_");
      findRelationsInKb(setAvailableRelations);
    }, [])

    const handleClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      handleSaveClassInstanceWithRelations(firstForm, form);
      setCreateRelationToEntityPopup(false);
      console.log(form.map(item => `${item.entity} - ${item.relation}`).join(', '));
      };

    const handleSaveClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      const newForm = [...form, { entity: entity, relation: relation }];
      setForm(newForm);
      setEntity("concept_");
      setRelation("nrel_");
      };

    const closeClick: React.MouseEventHandler<HTMLButtonElement> = async (event) => {
      await userClose();
      setCreateRelationToEntityPopup(false);
    };

    const handleChipDelete = (index: number) => {
      const newChipsValues = [...form];
      newChipsValues.splice(index, 1);
      setForm(newChipsValues);
    };


    return (
      <div className="popup">
        <h2>Создание экземпляра класса</h2>
        <button className='close_button' onClick={closeClick}>×</button>
        <div className="form">
            <h4>Сущность</h4>
            <AutoComplete
              value={entity}
              options={availableEntity}
              style={{ width: 480 }}
              filterOption={(inputValue, option) =>
                option?.label.toUpperCase().indexOf(inputValue.toUpperCase()) !== -1
              }
              onSelect={onSelectEntity}
              onChange={onChangeEntity}
              dropdownStyle={{zIndex: 10000}}
            >
              <FormInput />
            </AutoComplete>
            <h4>Отношение</h4>
            <AutoComplete
              value={relation}
              options={availableRelations}
              style={{ width: 480 }}
              filterOption={(inputValue, option) =>
                option?.label.toUpperCase().indexOf(inputValue.toUpperCase()) !== -1
              }
              onSelect={onSelectRelation}
              onChange={onChangeRelation}
              dropdownStyle={{zIndex: 10000}}
            >
              <FormInput />
            </AutoComplete>
            {form.map((value, index) => (
              <div className="chip-div" key={index}>
                <div className="chip">
                <span>{value.entity}</span>
                <span>—</span>
                <span>{value.relation}</span>
                </div>
                <span className="chip-delete" onClick={() => handleChipDelete(index)}>
                  ×
                </span>
              </div>
            ))}
            <button className="button next" onClick={handleSaveClick}>Добавить</button>
            </div>
            <button className="button save" onClick={handleClick}>
            Сохранить
            </button>
        
      </div>
    );
  };
