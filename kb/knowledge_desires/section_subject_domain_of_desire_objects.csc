section_subject_domain_of_desire_objects => nrel_main_ndtf: [Раздел. Предметная область объектов инфраструктуры] (* -< lang_ru;; *);;
section_subject_domain_of_desire_objects => nrel_main_ndtf: [Section. Subject domain of desire_objects] (* -< lang_en;; *);;
section_subject_domain_of_desire_objects <- sc_node_class;;

non_atomic_section -> section_subject_domain_of_desire_objects;;

section_subject_domain_of_desire_objects = [*
    section_subject_domain_of_desire_objects
    => nrel_main_ndtf;
        [Раздел. Предметная область объектов инфраструктуры] (* -< lang_ru;; *);;
        [Section. Subject domain of desire_objects] (* -< lang_en;; *);;
    <- subject_domain;
    -> rrel_maximum_studied_object_class:
        concept_desire;

    -> rrel_explred_relation:
        nrel_desire;;
    *];;

rrel_key_sc_element -> (section_subject_domain_of_desire_objects -> subject_domain_of_desire_objects)
(*
    => nrel_key_sc_element_order: (section_subject_domain_of_desire_objects -> concept_desire)
(*
    <- rrel_key_sc_element;;
*);;*);;