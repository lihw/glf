#ifndef KxLayoutWidget_H_
#define KxLayoutWidget_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxLayoutWidget : public QWidget
{
	Q_OBJECT;
public:
	~KxLayoutWidget();
	
	template <class T> static KxLayoutWidget* create(QWidget *parent = NULL);
	template <class T> T* getLayout() const;

	void setObjectName(const QString &name);
	
protected:
	virtual void childEvent(QChildEvent * event);

private:
	// Hide default constructor
	KxLayoutWidget(QWidget *parent = NULL);
};

template <class T> inline KxLayoutWidget* KxLayoutWidget::create(QWidget *parent)
{
	KxLayoutWidget *w = new KxLayoutWidget(parent);
	T* l = new T(w);
	w->setLayout(l);
	return w;
}

template <class T> inline T* KxLayoutWidget::getLayout() const
{
	return qobject_cast<T*>(layout());
}

#endif
