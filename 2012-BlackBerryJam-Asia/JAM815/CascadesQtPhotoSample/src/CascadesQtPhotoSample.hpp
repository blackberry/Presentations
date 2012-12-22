// Default empty project template
#ifndef CascadesQtPhotoSample_HPP_
#define CascadesQtPhotoSample_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class CascadesQtPhotoSample : public QObject
{
    Q_OBJECT
public:
    CascadesQtPhotoSample(bb::cascades::Application *app);
    virtual ~CascadesQtPhotoSample() {}
};


#endif /* CascadesQtPhotoSample_HPP_ */
