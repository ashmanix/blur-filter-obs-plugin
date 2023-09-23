#include "blur-filter-widget.hpp"

BlurFilterWidget::~BlurFilterWidget() {}

void BlurFilterWidget::RegisterSource()
{
	source_info.id = "ashmanix_plugin_blur_filter";
	source_info.type = OBS_SOURCE_TYPE_FILTER;
	source_info.output_flags = OBS_SOURCE_VIDEO;
	source_info.get_name = BlurFilterWidgetGetName;
	source_info.create = BlurFilterWidgetCreate;
	source_info.destroy = BlurFilterWidgetDestroy;
	source_info.video_render = BlurFilterWidgetRender;
	source_info.update = BlurFilterWidgetUpdate;
	source_info.get_properties = BlurFilterWidgetProperties;
	source_info.get_defaults2 = BlurFilterWidgetDefaults;

	obs_register_source(&source_info);
}

const char *BlurFilterWidget::BlurFilterWidgetGetName(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("BlurFilter");
}

void BlurFilterWidget::ChangeFilterSelection(struct filter_data *filterData)
{
	UNUSED_PARAMETER(filterData);
	long long filterIndex = filterData->selectedFilterIndex;
	obs_log(LOG_INFO, "Selected Filter Index: %ld", filterIndex);

	obs_enter_graphics();

	if (filterData->effect) {
		gs_effect_destroy(filterData->effect);
		obs_log(LOG_INFO, "Destroying filter effect!");
	}
	const char *effectPath =
		filterData->filterArray[filterIndex]->GetShaderFilePath();
	filterData->effect = gs_effect_create_from_file(effectPath, NULL);

	obs_leave_graphics();

	if (!filterData->effect) {
		blog(LOG_ERROR, "Could not load blur effect file '%s'",
		     effectPath);
	}

	// gs_effect_t *effect = filterData->effect;
	filterData->filterArray[filterIndex]->SetFilterParameters(
		filterData->effect);
	TogglePropertyGroupVisibility(filterData);
}

void *BlurFilterWidget::BlurFilterWidgetCreate(obs_data_t *settings,
					       obs_source_t *source)
{
	// UNUSED_PARAMETER(settings);
	struct filter_data *filterData =
		(struct filter_data *)bzalloc(sizeof(struct filter_data));

	filterData->filterArray.push_back(
		std::unique_ptr<BaseFilter>(new SimpleGaussianFilter()));
	filterData->filterArray.push_back(
		std::unique_ptr<BaseFilter>(new TestFilter()));

	filterData->context = source;

	// filterData->selectedFileName = bstrdup("box_blur.effect");
	ChangeFilterSelection(filterData); // Default to box blur
	obs_source_update(source, settings);

	return filterData;
}

void BlurFilterWidget::BlurFilterWidgetDestroy(void *data)
{
	struct filter_data *filterData = (struct filter_data *)data;
	if (filterData->effect) {
		obs_enter_graphics();
		gs_effect_destroy(filterData->effect);
		obs_leave_graphics();
	}
	bfree(data);
}

void BlurFilterWidget::BlurFilterWidgetUpdate(void *data, obs_data_t *settings)
{
	obs_log(LOG_INFO, "Updating Filter from update function");
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(settings);
	struct filter_data *filterData = (struct filter_data *)data;
	long long filterIndex = filterData->selectedFilterIndex;
	filterData->filterArray[filterIndex]->UpdateFilter(settings);
}

void BlurFilterWidget::UpdateFilterProperties(obs_properties_t *properties,
					      long long selectedFilterIndex)
{
	UNUSED_PARAMETER(properties);
	UNUSED_PARAMETER(selectedFilterIndex);
}

bool BlurFilterWidget::FilterSelectionChangeCallback(void *data,
						     obs_properties_t *props,
						     obs_property_t *list,
						     obs_data_t *settings)
{
	UNUSED_PARAMETER(props);
	// UNUSED_PARAMETER(list);
	// UNUSED_PARAMETER(settings);

	long long selectedFilterIndex =
		obs_data_get_int(settings, SETTING_BLUR_TYPE);

	struct filter_data *filterData = (struct filter_data *)data;

	if (selectedFilterIndex != filterData->selectedFilterIndex) {
		filterData->selectedFilterIndex = selectedFilterIndex;

		ChangeFilterSelection(filterData);

		obs_log(LOG_INFO, "Filter change!");
		obs_log(LOG_INFO, "Property Changed To: %s",
			obs_property_list_item_name(list, selectedFilterIndex));

		// obs_log(LOG_INFO, "Selected Filter Index: %ld",
		// 	selectedFilterIndex);
	}

	return true;
}

obs_properties_t *BlurFilterWidget::BlurFilterWidgetProperties(void *data)
{
	// Register all filter properties to the properties object
	// Each filter will have its own filter group which will be saved in its own class
	// When a filter is chosen from the dropdown then we will hide all other groups and just show the selected filters options
	struct filter_data *filterData = (struct filter_data *)data;
	// UNUSED_PARAMETER(filterData);
	if (filterData) {
		filterData->mainProperties = obs_properties_create();

		obs_property_t *filterDropdownProperty =
			obs_properties_add_list(
				filterData->mainProperties, SETTING_BLUR_TYPE,
				obs_module_text("BlurTypeDropdownSelectTitle"),
				OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
		obs_property_list_add_int(
			filterDropdownProperty,
			obs_module_text("BlurTypeSelectOptionBox"), box);
		obs_property_list_add_int(
			filterDropdownProperty,
			obs_module_text("BlurTypeSelectOptionSimpleGaussian"),
			simple_gaussian);

		obs_property_set_modified_callback2(
			filterDropdownProperty, FilterSelectionChangeCallback,
			filterData);

		std::string filterStringName = "filter-";
		for (size_t i = 0; i < filterData->filterArray.size(); i++) {
			filterData->filterArray[i]->SetProperties(
				filterData->mainProperties,
				filterStringName.append(std::to_string(i)));

			TogglePropertyGroupVisibility(filterData);
		}
		return filterData->mainProperties;
	}

	return nullptr;
}

void BlurFilterWidget::BlurFilterWidgetDefaults(void *data,
						obs_data_t *settings)
{
	UNUSED_PARAMETER(settings);
	UNUSED_PARAMETER(data);
}

void BlurFilterWidget::BlurFilterWidgetRender(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(data);
	UNUSED_PARAMETER(effect);
	struct filter_data *filterData = (struct filter_data *)data;
	long long filterIndex = filterData->selectedFilterIndex;
	obs_source_t *context = filterData->context;
	filterData->filterArray[filterIndex]->Render(context);
}

BlurFilterWidget::BlurFilterWidget() {}

void BlurFilterWidget::TogglePropertyGroupVisibility(filter_data *data)
{
	for (size_t i = 0; i < data->filterArray.size(); i++) {
		if (data->selectedFilterIndex >= 0 &&
		    i != (size_t)data->selectedFilterIndex) {
			data->filterArray[i]->HidePropertiesGroup();
		} else {
			data->filterArray[i]->ShowPropertiesGroup();
		}
	}
}
